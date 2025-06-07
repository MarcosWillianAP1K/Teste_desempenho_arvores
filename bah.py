import random

# Define o número de números aleatórios a gerar
numero_de_numeros = 1000000

# Cria um conjunto (set) para armazenar os números aleatórios
numeros_aleatorios = set()

# Enquanto o conjunto não tiver o número desejado de elementos
while len(numeros_aleatorios) < numero_de_numeros:
    # Gera um número aleatório
    numero_aleatorio = random.randint(1, numero_de_numeros)
    # Adiciona o número ao conjunto
    numeros_aleatorios.add(numero_aleatorio)

# Converte para lista e embaralha
numeros_lista = list(numeros_aleatorios)
random.shuffle(numeros_lista)

# Abre o arquivo para escrita
with open('numeros_aleatorios.txt', 'w') as f:
    # Escreve cada número no arquivo, um por linha
    for numero in numeros_lista:
        f.write(str(numero) + '\n')

print("Arquivo 'numeros_aleatorios.txt' criado com sucesso.")