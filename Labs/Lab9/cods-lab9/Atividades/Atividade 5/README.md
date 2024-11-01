# Atividade 5 - Laboratório 9

Inicialmente o fiz o código para gerar uma sequência com números aleatórios (alguns podendo se repetir), o código fazia a contagem correta dos números primos nessa sequência (considerando o caso de primos duplicados também) mas para facilitar os testes e validação dos monitores e professora alterei para um código que gera uma sequências não-aleatória de números e conta a quantidade de números primos, criando M goroutines. 

## Casos de teste

Usei esses dois casos por ser de fácil validar isso pela internet:
- sequência com tamanho **100** contém **25** primos;
- sequência com tamanho **200** contém **46** primos;