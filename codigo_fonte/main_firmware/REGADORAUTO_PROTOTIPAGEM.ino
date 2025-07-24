#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>



#define PIN_SENSOR_UMIDADE 34
#define PIN_RELE 18

#define UMIDADE_SECO 3300
#define UMIDADE_UMIDO 1400
#define LIMIAR_UMIDADE_PORCENTAGEM 35

#define DHTPIN 15
#define DHTTYPE DHT11

#define SD_CS 5
#define SD_MOSI 23
#define SD_MISO 19
#define SD_CLK 13


const char* ssid = "Pentecostes";
const char* password = "40044595";

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
WebServer server(80);

int umidadePercentual = 0;
String statusValvula = "DESLIGADA";
float temperatura = 0.0;
float umidadeAr = 0.0;
File arquivo;

unsigned long tempoAnteriorLCD = 0;
const unsigned long intervaloLCD = 15000; // 15 segundos


// Página HTML com dados extras
const char* paginaHTML = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <title>Monitoramento de Umidade e Temperatura</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    body {
      margin: 0;
      font-family: 'Segoe UI', sans-serif;
      background: #eef2f3;
      color: #333;
    }
    header {
      background-color: #00695c;
      color: white;
      padding: 20px;
      text-align: center;
    }
    main {
      padding: 30px;
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    .info-box {
      background: white;
      box-shadow: 0 2px 5px rgba(0,0,0,0.2);
      border-radius: 10px;
      padding: 20px;
      margin-bottom: 20px;
      width: 90%;
      max-width: 600px;
    }
    .info-box h2 {
      margin-top: 0;
    }
    #statusValvula {
      font-weight: bold;
      color: green;
    }
    .icon {
      font-size: 1.5em;
      margin-right: 8px;
      vertical-align: middle;
    }
    canvas {
      max-width: 100%;
    }
  </style>
</head>
<body>
  <header>
    <h1>🌱 Monitoramento do Solo</h1>
  </header>

  <main>
    <div class="info-box">
      <h2><span class="icon">💧</span>Umidade: <span id="umidade">--</span> %</h2>
      <h2><span class="icon">🌡️</span>Temperatura: <span id="temperatura">--</span> °C</h2>
      <p><span class="icon">🚿</span>Status da Válvula: <span id="statusValvula">--</span></p>
    </div>

    <div class="info-box">
      <canvas id="graficoUmidade"></canvas>
    </div>
  </main>

  <script>
    const ctx = document.getElementById('graficoUmidade').getContext('2d');
    const chart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: [],
        datasets: [
          {
            label: 'Umidade (%)',
            borderColor: '#00695c',
            backgroundColor: 'rgba(0,105,92,0.1)',
            data: [],
            tension: 0.3,
            fill: true,
            yAxisID: 'y1'
          },
          {
            label: 'Temperatura (°C)',
            borderColor: '#ff9800',
            backgroundColor: 'rgba(255,152,0,0.1)',
            data: [],
            tension: 0.3,
            fill: true,
            yAxisID: 'y2'
          }
        ]
      },
      options: {
        responsive: true,
        scales: {
          y1: {
            type: 'linear',
            position: 'left',
            beginAtZero: true,
            max: 100,
            title: {
              display: true,
              text: 'Umidade (%)'
            }
          },
          y2: {
            type: 'linear',
            position: 'right',
            beginAtZero: true,
            max: 50,
            title: {
              display: true,
              text: 'Temperatura (°C)'
            },
            grid: {
              drawOnChartArea: false
            }
          }
        }
      }
    });

    async function atualizarUmidade() {
      try {
        const response = await fetch('/umidade');
        const data = await response.json();
        const umidade = data.umidade.toFixed(1);
        const temperatura = data.temperatura.toFixed(1);
        const valvula = data.valvula;
        const timestamp = new Date().toLocaleTimeString();

        document.getElementById('umidade').textContent = umidade;
        document.getElementById('temperatura').textContent = temperatura;

        const statusEl = document.getElementById('statusValvula');
        statusEl.textContent = valvula;
        statusEl.style.color = (valvula === "LIGADA") ? "#d32f2f" : "green";

        if (chart.data.labels.length > 20) {
          chart.data.labels.shift();
          chart.data.datasets[0].data.shift();
          chart.data.datasets[1].data.shift();
        }

        chart.data.labels.push(timestamp);
        chart.data.datasets[0].data.push(umidade);
        chart.data.datasets[1].data.push(temperatura);
        chart.update();
      } catch (error) {
        console.error("Erro ao obter dados:", error);
      }
    }

    setInterval(atualizarUmidade, 15000);  // atualiza a cada 15 segundos (para teste constante vizualização)
    atualizarUmidade();
  </script>
</body>
</html>
)rawliteral";

void salvarDadosNoSD() {
  String linha = String(umidadePercentual) + ";" +
                 String(temperatura, 1) + ";" +
                 String(umidadeAr, 1) + ";" +
                 statusValvula + ";" +
                 String(millis() / 60000) + "min\n";

  arquivo = SD.open("/dados.csv", FILE_APPEND);
  if (arquivo) {
    arquivo.print(linha);
    arquivo.close();
    Serial.println("Dados salvos no SD.");
  } else {
    Serial.println("Erro ao abrir dados.csv");
  }
}

//////////////////////////////////////////////////////////////////////
// utilizando previsão da IA, com base em dados csv anteriores ///////
//////////////////////////////////////////////////////////////////////
String preverStatusValvulaIA(float umidadeSolo, float temperatura) {
  if (umidadeSolo < 35 && temperatura > 25.0) {
    return "LIGADA";
  } else {
    return "DESLIGADA";
  }
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void setup() {
  
  Serial.begin(115200);
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, HIGH); // Inicialmente desligado

  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Conectando WiFi");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(4000);
  lcd.clear();

  // Tenta inicializar o cartão SD
  SPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("[ERRO] Falha ao inicializar o cartão SD!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro no cartao SD");
    delay(3000);
  } else {
    Serial.println("[INFO] Cartão SD inicializado com sucesso.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SD OK!");
    delay(1500);

    // Cria arquivo CSV se não existir
    if (!SD.exists("/dados.csv")) {
      arquivo = SD.open("/dados.csv", FILE_WRITE);
      if (arquivo) {
        arquivo.println("UmidadeSolo(%);Temperatura(C);UmidadeAr(%);Valvula;Minuto");
        arquivo.close();
        Serial.println("[INFO] Arquivo 'dados.csv' criado.");
      } else {
        Serial.println("[ERRO] Não foi possível criar 'dados.csv'.");
      }
    } else {
      Serial.println("[INFO] Arquivo 'dados.csv' já existe.");
    }
  }

  server.on("/", []() {
    server.send(200, "text/html", paginaHTML);
  });

  server.on("/umidade", []() {
    int leitura = analogRead(PIN_SENSOR_UMIDADE);
    umidadePercentual = map(leitura, UMIDADE_SECO, UMIDADE_UMIDO, 0, 100);
    umidadePercentual = constrain(umidadePercentual, 0, 100);

    temperatura = dht.readTemperature();
    umidadeAr = dht.readHumidity();

// Decisao criada pela IA: ERRO
//statusValvula = preverStatusValvulaIA(umidadePercentual, temperatura);
//digitalWrite(PIN_RELE, statusValvula == "LIGADA" ? HIGH : LOW);



//    if (umidadePercentual < LIMIAR_UMIDADE_PORCENTAGEM) {
//      digitalWrite(PIN_RELE, HIGH);
//      statusValvula = "LIGADA";
//    } else {
//      digitalWrite(PIN_RELE, LOW);
//      statusValvula = "DESLIGADA";
//    }

    // LCD
    lcd.setCursor(0, 0);
    lcd.print("U:");
    lcd.print(umidadePercentual);
    lcd.print("% T:");
    lcd.print((int)temperatura);
    lcd.setCursor(0, 1);
    lcd.print("V:");
    lcd.print(statusValvula == "LIGADA" ? "LIG" : "DES");
    lcd.print(" UA:");
    lcd.print((int)umidadeAr);
    lcd.print("  ");

    salvarDadosNoSD(); // gravação automática no cartão SD

    String json = "{\"umidade\": " + String(umidadePercentual) +
                  ", \"valvula\": \"" + statusValvula +
                  "\", \"temperatura\": " + String(temperatura, 1) +
                  ", \"umidadeAr\": " + String(umidadeAr, 1) + "}";

    server.send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  server.handleClient();

  if (millis() - tempoAnteriorLCD >= intervaloLCD) {
    tempoAnteriorLCD = millis();

    int leitura = analogRead(PIN_SENSOR_UMIDADE);
    umidadePercentual = map(leitura, UMIDADE_SECO, UMIDADE_UMIDO, 0, 100);
    umidadePercentual = constrain(umidadePercentual, 0, 100);

    temperatura = dht.readTemperature();
    umidadeAr = dht.readHumidity();

    statusValvula = preverStatusValvulaIA(umidadePercentual, temperatura);
    digitalWrite(PIN_RELE, statusValvula == "LIGADA" ? HIGH : LOW);

    // Atualiza LCD
    lcd.setCursor(0, 0);
    lcd.print("U:");
    lcd.print(umidadePercentual);
    lcd.print("% T:");
    lcd.print((int)temperatura);
    lcd.setCursor(0, 1);
    lcd.print("V:");
    lcd.print(statusValvula == "LIGADA" ? "LIG" : "DES");
    lcd.print(" UA:");
    lcd.print((int)umidadeAr);
    lcd.print("  ");

    // Salva dados no SD
    salvarDadosNoSD();
    Serial.println("Atualização automática (LCD + SD) a cada 15s.");
  }
}

