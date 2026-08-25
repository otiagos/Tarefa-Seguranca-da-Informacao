#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define PORT 31041 // Macro usado para definir o número da porta
#define SERVER_IP "127.0.0.1" // Macro usado para definir o endereço IP

int main(void) {
	char string[1024] = {0}; // Declaração de string do tipo char de 1KB
	int sock = 0, received = 0; // Declaração das variáveis sock, para o uso como descritor de arquivo, e received, para receber o número de bytes
	struct sockaddr_in sa; // Declaração de alias da struct sockaddr_in, para configurar o endereço IP e porta no socket

	memset(&sa, 0, sizeof(sa)); // Inicialização(limpeza) da struct

	sa.sin_family = AF_INET; // Configuração da struct sendo da família IPv4
	sa.sin_port = htons(PORT); // Configuração da porta na struct

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // Criação do socket para conexão
		printf("Erro ao criar socket.\n");

		return 1;
	}

	if (inet_pton(AF_INET, SERVER_IP, &sa.sin_addr) <= 0) { // "Tradução" do endereço IP configurado no socket de string para binário
		printf("Endereço inválido ou não suportado.\n"); // Caso inet_pton retorne -1, a mensagem de erro será mostrada
		close(sock); // O socket será fechado em caso de erro

		return 1; // A execução do programa será finalizada e retornará o código de erro
	}

	if ((connect(sock, (struct sockaddr *) &sa, sizeof(sa))) < 0) { // A partir do endereço IP e da porta configurados na struct, será verificada a conexão
		printf("Erro em conectar ao servidor.\n"); // Se houver erros a mensagem será mostrada
		close(sock); // O socket criado será fechado em caso de erro

		return 1; // O programa será finalizado e será retornado o código -1 em caso de erro na conexão
	}

	printf("Conectado.\n");

	/* Aoós a conexão estabelecida, o for terá um looping infinito, onde o mesmo receberá passivamente quaisquer strings digitadas
	 * pelo par conectado. A função recv receverá o tamanho total da string, de 1KB, e em caso de erro, será mostrada a mensagem,
	 * o socket de conexão será fechado e será retornado o código de erro -1, além também de caso o número de bytes seja 0. Se tudo
	 * ocorreu corretamente, a string recebida terá o caractere NULL inserido no último elemento do array, onde será usado para que
	 * posteriormente a mensagem recebida seja mostrada no output. Após a mensagem ser mostrada, a string será limpada para a espera
	 * de uma nova. Após o outro par digitar exit, a conexão será interrompida, o socket de conexão será fechado e a execução será
	 * encerrada com o código 0.
	 */
	for(;;) {
		if ((received = recv(sock, string, 1023, 0)) < 0){
			printf("Erro ao receber dados.\n");
			close(sock);

			return 1;
		}
		else if (received == 0) {
			printf("Conexão encerrada.\n");
			close(sock);

			return 0;
		}
		else {
			string[received] = '\0';

			printf("%s\n", string);
		}
		memset(string, 0, strlen(string));
	}

	close(sock);

	printf("\n\nFim do programa\n");

	return 0;
}
