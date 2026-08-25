# Tarefa-Seguranca-da-Informacao

Alunos:
Tiago Soares Costa de Oliveira - 201210797


Foi utilizado o compilador GCC (GNU Complete Compiler) para a compilação dos dois arquivos .c. 
Para a compilação foram usados os comandos gcc prog01.c -o prog01 e gcc prog02.c -o prog02, ambos 
com o objetivo de criar dois arquivos executáveis com a biblioteca glic do Linux.

Após a compilação e execução dos executáveis, tanto o processo do lado "vítima" quanto o processo do lado 
"invasor" foram executados simultaneamente, onde enquanto no processo "vítima" rodava de forma passiva quando ao 
digitar no teclado, quando apertava Enter todos os caracteres digitados no teclado eram enviados, via socket, 
para o processo "invasor", onde o mesmo recebia os caracteres, também via socket, e mostrava os mesmos pela 
saída padrão, a tela. 

