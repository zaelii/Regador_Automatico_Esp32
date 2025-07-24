# 📦 Lista de Materiais de Software

## 🔧 Firmware ESP32-WROOM-32

### 📚 Bibliotecas Utilizadas

- **Conectividade**
  - `WiFi.h` – Estabelece a conexão Wi-Fi com a rede local.
  - `WebServer.h` – Criação do servidor HTTP local para interface web.

- **Sensores**
  - `DHT.h` – Leitura da temperatura e umidade do ar (sensor DHT11).
  - `analogRead()` – Leitura da umidade do solo via pino analógico.

- **Display LCD**
  - `Wire.h` – Comunicação I2C com periféricos.
  - `LiquidCrystal_I2C.h` – Controle do display LCD 16x2 com backlight azul.

- **Armazenamento**
  - `SPI.h` – Comunicação SPI com o cartão SD.
  - `SD.h` – Leitura e escrita em arquivos `.csv` no cartão microSD.

- **Diversos**
  - `Arduino.h` – Base de funções essenciais da linguagem C/C++ para Arduino.
  - `String` – Manipulação de textos e dados JSON na resposta web.

---

## 🧠 Script Python (Treinamento da IA)

### 📚 Bibliotecas Utilizadas

- **Manipulação de Dados**
  - `pandas` – Leitura, limpeza e preparação dos dados históricos coletados do sensor.

- **Modelagem Preditiva**
  - `scikit-learn` – Treinamento do modelo de árvore de decisão (DecisionTreeClassifier).
  - `joblib` – Salvamento e carregamento do modelo treinado.

- **Avaliação**
  - `classification_report` e `accuracy_score` – Métricas de desempenho do modelo.

- **Visualização**
  - `matplotlib` – Geração da visualização da árvore de decisão (plot_tree).

---

## 💡 Observações Importantes

- O modelo de IA atualmente é simples e interpretado manualmente no código do ESP32 por meio de `if-else`, simulando uma árvore de decisão.
- A estrutura de gravação no cartão SD permite futura aplicação de modelos mais complexos (ex: TensorFlow Lite) com base em dados reais.
- Todo o firmware está otimizado para operar offline e online, exibindo dados no display e também via navegador (servidor web local).
