# **Observações**

Reaproveitei uma boa parte dos códigos oferecidos no lab pela professora, tomando a liberdade para alterá-los em algumas partes para
atender ao objetivo do Lab. 

Seguimos a pedida de dar preferência aos escritores, e para isso criei variáveis para registrar quantos escritores estão escrevendo
(sempre será 1 pois não podemos ter múltiplos escritores escrevendo simultaneamente) e quantos estão querendo escrever. Uma thread
leitor só pode dar início a sua leitura quando não houver nenhum escritor escreve ou querendo escrever. 

Tomei a liberdade e defini novas porcentagens de operações de consulta 40%, inserção 30% e remoção 30%. Isso porque as porcentagens definidas anteriormente só definiam 1% para inserção e 1% para remoção o que dificultava os testes.

# Sobre as funções presentes no **entrada_saida.h**:

Algumas variáveis de condição foram criadas;

1) `pthread_cond_t cond_ler, cond_escrever;`
2) `int leitores = 0; int escritores = 0; int querendoEscrever = 0; int leitoresAguardando = 0;` 

Essas variáveis são incrementadas e decrementadas dentro das funções definidas respeitando o padrão thread-safe. 

Implementei quatro funções:

1) EntradaLeitura(): As threads de leitura executam quando não temos nenhum escritor escrevendo e ou querendo escrever, caso 
contrário, elas entram em wait até que recebam um sinal e verifiquem que não existem escritores atuando. Adicionei um print antes que as threads entrassem em "wait" para mostrar quantos são os leitores esperando naquele momento.

2) SaiLeitura(); A saída é bem simples, mas também adicionei um print para atualizar o usuário de quantos leitores ainda estão
aguardando.

3) EntradaEscrita(); As threads de escrita executam quando não temos leitores lendo e outro escritor escrevendo, caso contrário, 
elas entram em wait até que recebam um sinal e verifiquem que não existem leitores ou outro escritor atuando. 

4) SaiEscrita(); Novamente, a saída de um escritor é simples, e também temos um print de quantos escritores estão aguardando.