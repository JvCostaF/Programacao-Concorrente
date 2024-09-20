def ler_arquivo_binario(caminho_arquivo):
    try:
        with open(caminho_arquivo, 'rb') as arquivo:
            conteudo = arquivo.read()
            print("Arquivo lido com sucesso!")
            return conteudo
    except FileNotFoundError:
        print("Arquivo não encontrado.")
    except Exception as e:
        print(f"Ocorreu um erro ao ler o arquivo: {e}")

# Exemplo de uso
caminho = 'C:\Users\joaov\UFRJ\Prog Conc\Programacao-Concorrente\Lab3\Atividade 1\saidaTeste.bin'
conteudo = ler_arquivo_binario(caminho)

if conteudo:
    # Se quiser imprimir o conteúdo como texto:
    print(conteudo)
    # Se quiser mostrar como lista de bytes:
    print(list(conteudo))
