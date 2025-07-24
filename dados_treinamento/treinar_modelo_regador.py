import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import classification_report, accuracy_score
import joblib
import matplotlib.pyplot as plt

# Carregar o arquivo CSV
df = pd.read_csv("dados.csv", sep=";")

# Remover colunas desnecessárias e lidar com possíveis valores ausentes
df.dropna(inplace=True)
df = df[df["Valvula"].isin(["LIGADA", "DESLIGADA"])]

# Mapeamento da variável alvo para valores numéricos
df["Valvula"] = df["Valvula"].map({"DESLIGADA": 0, "LIGADA": 1})

# Separar features e target
X = df[["UmidadeSolo(%)", "Temperatura(C)"]]
y = df["Valvula"]

# Divisão treino/teste
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Criar e treinar o modelo
modelo = DecisionTreeClassifier(max_depth=3, random_state=42)
modelo.fit(X_train, y_train)

# Avaliação
y_pred = modelo.predict(X_test)
print("\nRelatório de Classificação:")
print(classification_report(y_test, y_pred))
print("Acurácia:", accuracy_score(y_test, y_pred))

# Salvar modelo
joblib.dump(modelo, "modelo_valvula.joblib")
print("\nModelo salvo como 'modelo_valvula.joblib'")

# Visualizar árvore de decisão (opcional)
from sklearn.tree import plot_tree

plt.figure(figsize=(10,6))
plot_tree(modelo, feature_names=X.columns, class_names=["DESLIGADA", "LIGADA"], filled=True)
plt.title("Árvore de Decisão - Regador Automático")
plt.show()
