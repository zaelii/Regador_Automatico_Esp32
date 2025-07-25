# 🌱 Regador Automático com ESP32 + IA

Este projeto é um sistema de irrigação automatizado que utiliza o **ESP32**, sensores de umidade do solo, temperatura e umidade do ar (DHT11), além de um display LCD I2C e um cartão microSD. A principal inovação é a integração de uma **Inteligência Artificial (IA)** treinada com dados reais, que decide automaticamente quando ligar a válvula de irrigação.

---

## 🎯 Objetivo

Automatizar a irrigação de plantas com base em dados ambientais coletados por sensores e decisões inteligentes com IA embarcada. O sistema também registra os dados em um cartão SD e permite o monitoramento em tempo real via página web.

---

## 🔧 Componentes Utilizados

| Componente            | Função                                                 |
|------------------------|--------------------------------------------------------|
| **ESP32-WROOM-32**     | Microcontrolador principal                            |
| **Sensor de Umidade**  | Mede a umidade do solo (analógico)                    |
| **Sensor DHT11**       | Mede temperatura e umidade do ar                      |
| **LCD I2C 16x2**        | Exibe informações locais                             |
| **MicroSD Card**       | Armazena dados históricos em CSV                     |
| **Módulo Relé**        | Controla a válvula de irrigação                       |
| **Wi-Fi embutido**     | Permite acesso remoto à interface web                |

---

## 🧠 Inteligência Artificial

A IA foi treinada com dados reais (`dados.csv`) contendo leituras de umidade, temperatura e estado da válvula. Utilizamos um modelo de **árvore de decisão** com a biblioteca `scikit-learn`.

### 🔗 Processo de Treinamento

1. **Coleta de dados:** Registrados automaticamente no cartão SD.
2. **Treinamento (offline):** Script Python treina e exporta o modelo com `joblib`.
3. **Inferência (offline):** Script simples realiza previsão da válvula usando os dados lidos em tempo real.

---

## 🖥️ Como Funciona

### 🔧 Configuração Inicial

1. Conecte-se a essa rede pelo seu computador ou celular (não é necessário internet).
2. Acesse `O IP QUE SERÁ FORNECIDO NO DISPLAY` pelo navegador.
3. Configure o Nome e senha do Wi-Fi da sua casa

---

## 🌐 Conectividade

- 🔌 **Offline por padrão:** O sistema funciona normalmente sem internet, realizando coleta, exibição e registro dos dados localmente.

---

## 📦 Processo de Funcionamento

1. O ESP32 coleta dados dos sensores de umidade do solo e ambiente.
2. Os dados são exibidos no LCD I2C e registrados no cartão SD em formato `.csv`.
3. Uma interface web local exibe os dados e gráficos dinâmicos em tempo real.
4. Scripts Python podem ser executados para prever o status da válvula com base nos dados coletados.
5. (Opcional) Notificações podem ser enviadas via Telegram, caso configurado.

---

## 📂 Organização do Projeto

```bash
REGADORAUTO_PROTOTIPAGEM/
├── codigo_fonte/
│   └── main_firmware/
│       └── REGADORAUTO_PROTOTIPAGEM.ino   # Código principal do ESP32
├── dados_treinamento/                     # Scripts e modelo de IA
│   ├── treinar_modelo.py
│   ├── prever_valvula.py
│   └── modelo_ia.joblib
├── diagramas/                             # Diagramas do projeto
├── fotos_videos/                          # Fotos e vídeos do protótipo
├── interface/                             # Interface web (HTML/CSS/JS)
├── materiais_software/                    # Materiais e softwares auxiliares
├── requisitos/                            # Documentação de requisitos
├── README.md                              # Documentação principal
```
---  

## 🌐 Interface Web

Uma interface moderna em HTML/CSS/JS exibe:

- Umidade do solo
- Temperatura ambiente
- Status da válvula
- Gráficos dinâmicos com Chart.js

A interface é atualizada a cada 15 segundos via `fetch()` e é hospedada localmente pelo ESP32.

---

## 🛠️ Tecnologias Utilizadas

- **Arduino Framework (ESP32)**
- **Python 3.12** + `pandas`, `scikit-learn`, `joblib`
- **HTML/CSS/JS** + Chart.js
- **SPI / I2C** (comunicação com SD e LCD)

---

## 📈 Status do Projeto

- [x] Integração de todos os componentes
- [x] Coleta e salvamento de dados via SD
- [x] Interface web local com gráfico dinâmico
- [x] Exibição no LCD I2C
- [x] Script de IA para previsão offline

---

## 📷 Capturas de Tela

### Interface Web
![Interface Web](/interface/Interface%20remoto%202.jpeg)

### LCD em funcionamento
<img src="fotos_videos/Display Lcd.jpeg" alt="LCD I2C - Alternativo" width="400"/>

### Protótipo físico
![Protótipo](fotos_videos/Todos%20os%20Componentes%20Juntos%202.jpeg)

### Demonstração em Vídeo

[Assista ao vídeo de demonstração](https://youtu.be/cQut4msHBkw)

---

Autores: Disraeli Filho, Roberto Filho, Matheus Carneiro
