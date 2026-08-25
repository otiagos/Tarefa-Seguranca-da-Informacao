#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

int main(void) {
	char string[1024] = {0};
	int sock = 0;
	struct sockaddr_in victim, invader;
	socklen_t invaderLen = sizeof(invader);

	memset(&victim, 0, sizeof(victim)); // Inicialização do endereço do socket victim
	memset(&invader, 0, sizeof(invader)); // Inicialização do endereço do socket invader

	victim.sin_family = AF_INET; // Configuração do IPv4
	victim.sin_port = htons(31041); // Transforma o número da porta em bytes
	victim.sin_addr.s_addr = htonl(INADDR_ANY); // O host victim aceita qualquer conexão, semelhante a um servidor, além de transformar INADDR_ANY em bytes

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // A função socket, caso não aloque uma a variável em uma posição no descritor de arquivo, retorna -1
		printf("Erro ao criar socket.\n"); // Imprime a mensagem de erro

		return 1; // Finaliza a execução de main com erro
	}

	if (bind(sock, (struct sockaddr *) &victim, sizeof(victim)) < 0) { // Após a criação do socket, é criada uma ligação entre ele e a porta e endereço IP victim
		printf("Erro ao ligar socket.\n"); // Caso bind retorne -1, a mensagem de erro é mostrada,
		close(sock);					   // o socket, no caso o descritor de arquivo, é fechado para evitar que a porta fique aberta

		return 1;						  // e a execução é finalizada com erro
	}

	if ((listen(sock, 5) < 0)) { // Após a ligação bem-sucedida, o socket fica na escuta, a espera de novas conexões na determinada porta,
		printf("Erro ao ouvir socket.\n"); // caso houver erros na escuta, a mensagem será mostrada
		close(sock);

		return 1;  						  // e a execução será finalizada com erro
	}

	printf("\nAguardando conexão...\n");

	int attackSock = accept(sock, (struct sockaddr *) &invader, &invaderLen); // Após a escuta receber uma nova conexão, accept criará um novo socket a partir desta nova
	if (attackSock < 0) {													  // criando também um novo descritor de arquivo
		printf("Erro ao aceitar conexão.\n"); // Caso houver erros durante a criação do socket, a mensagem de erro será mostrada
		close(sock); // O socket da escuta de novas conexões será fechado

		return 1; // A execução será finalizada com o código de erro
	}

	printf("Conexão estabelecida...\n");

	/* Essa parte terá um looping for infinito, onde a conexão será estabelecida enquanto o usuário não digitar exit.
	 * É utilizada apenas uma conexão TCP após a mesma ser estabelecida na função accept. A string digitada em fgets
	 * conterá a quantidade de caracteres junto com o caractere de fim de string '\0', onde posteriormente será verificada
	 * a string digitada em fgets e, caso seja exit, tanto o socket da conexão quando o socket da escuta serão fechados.
	 * Caso contrário, a string será enviada pela função send ao invader. Um detalhe que garante que a conexão seja TCP é
	 * o uso da função send, ao passo que sendto é usado preferencialmente em conexões UDP.
	 */
	for(;;) {
		fgets(string, 1024, stdin);
		string[strcspn(string, "\n")] = '\0';

		if (strcmp(string, "exit") == 0) {
			close(attackSock);
			break;
		}

		if ((send(attackSock, string, strlen(string), 0)) < 0)
			printf("Erro ao enviar dados.\n");

		memset(string, 0, 1024);
	}

	close(sock);

	printf("\nFim do programa\n");

	return 0;
}
