# Atenção

Não pude implementar o segundo algoritmo devido à falta de tempo, além de uma maneira de desenvolver que se provou bastante exigente na parte de depuração de código. Além disso, não posso garantir que o código está escrevendo o arquivo de maneira correta, isso devido a um erro que o MARS estava encontrando durante a syscall de escrita de arquivo, em reclamava de acesso de memória ilegal. Porém, acredito que talvez seja um erro do meu código, por cause de uma má interpretação por minha parte. Estava tentando escrever um char, mas na verdade, eu deveria estar escrevendo um buffer.

Porém se o código for executado no MARS, é possível ver no output a entrada adequadamente ordenada pelo algoritmo.

-------

Devo agradecer ao projeto mips2c que me permitiu depurar melhor a aplicação, além do algoritmo de comparação de strings do 'techie delight' que usei como base para a minha implementação. Também devo agradecer ao 'I-Smith' que implementou um algoritmo de quicksort para ser usando em cima de strings. Por ultimo, gostaria de agradecer ao 'Sylvain Defresne' que apresentou uma bela implementação da função lmalloc no GitHub.