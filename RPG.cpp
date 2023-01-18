#include <cstddef>
#include <iostream>
#include <locale.h>
#include <stdexcept>
#include <string>
#include <time.h>

#ifdef _WIN32
#define PAUSE(msg) system("pause")
#define CLEAR() system("cls")
#else
#define PAUSE(msg) system("echo " msg " ; read _")
#define CLEAR() system("clear")
#endif

using namespace std;

struct Arma {
	int danoMinimo;
	int danoMaximo;
};

struct Jogador {
	string nome;
	int nivel = 42;
	int vida = 0;
	Arma arma{ 10, 20 };
	int posicaoY = 0;
	int posicaoX = 0;
};

struct Inimigo {
	string nome;
	int vida = 20;
	Arma arma = { 10,15 };
};

struct Bloco {
	bool isBloqueado;
	bool isInimigo;
	Inimigo* inimigoBloco;
};

struct Mapa {
	int altura;
	int largura;
	Bloco** matriz;
};

struct Fase {
	string nome;
	Mapa mapa = {};
	int numeroInimigos = {};
	Inimigo* inimigo = {};
};

void imprimirMapa(Fase fase, Jogador jogador) {
	for (int i = 0; i < fase.mapa.altura; i++) {
		for (int j = 0; j < fase.mapa.largura; j++) {
			if (i == jogador.posicaoY && j == jogador.posicaoX) {
				cout << "&";
			}
			else if (fase.mapa.matriz[i][j].isInimigo) {
				// cout << fase.mapa.matriz[i][j].inimigoBloco->nome;
				cout << "@";
			}
			else if (fase.mapa.matriz[i][j].isBloqueado) {
				cout << "o";
			}
			else {
				cout << "_";
			}
			cout << " ";
		}
		cout << endl;
	}
	cout << "["<< jogador.nome <<"]-["<< jogador.vida <<"HP]\n";
	cout << "& = Jogador / @ = Inimigo / o = Pedra / - = Caminho\n";
}

Mapa criarMapa(int altura, int largura) {
	Mapa mapa{};

	mapa.altura = altura;
	mapa.largura = largura;

	mapa.matriz = new Bloco * [mapa.altura]; // CRIA A MATRIZ DINÂMICA QUE É O MAPA!

	for (int i = 0; i < mapa.altura; i++) {
		mapa.matriz[i] = new Bloco[mapa.largura]; // CRIA AS CAMADAS DO ARRAY BIDIMENSIONAL!
	}

	for (int i = 0; i < mapa.altura; i++) { // LOOP PARA COLOCAR SE É OU TERRA OU PEDRA!
		for (int j = 0; j < mapa.largura; j++) {
			int numeroSorteiro = 0; // DECLARAÇÃO DO NÚMERO PARA SORTEIO SE É INIMIGO!

			mapa.matriz[i][j].isBloqueado = false; // ATRIBUI COMO PADRÃO Q É CAMINHO ATÉ QUE SE COLOQUE O CONTRARIO!

			numeroSorteiro = (rand() % 9); // CALCULO DE 20% (0 OU 1) PARA SER PEDRA OU 80% (3,4,5,6,7,8,9) PARA SER TERRA!

			if (numeroSorteiro == 0 || numeroSorteiro == 1) {
				mapa.matriz[i][j].isBloqueado = true; // ATRIBUI QUE É PEDRA!
			}
		}
	}
	mapa.matriz[0][0].isBloqueado = true; // POSIÇÃO INICIAL DO JOGADOR NÃO PODE SER PEDRA!

	return mapa; // RETORNA O MAPA!
}

Inimigo* criarInimigos(int numeroInimigos, int tipo){
	Inimigo* inimigos = new Inimigo[numeroInimigos];
	if(tipo == 1){
		for (int i = 0; i < numeroInimigos; i++) {
			if (i == 0) {
				inimigos[0] = { "Coringa", 40, {10, 18} };
			}
			else if (i == 1) {
				inimigos[1] = { "Pinguim", 20, {6, 10} };
			}
			else if (i == 2) {
				inimigos[2] = { "Duas Caras", 10, {6, 10} };
			}
			else if (i == 3) {
				inimigos[3] = { "Charada", 20, {6, 10} };
			}
			else if (i == 4) {
				inimigos[4] = { "Ra's al Ghul", 20, {10, 14} };
			}
			else {
				inimigos[i].nome = "Bêbado " + std::to_string(i + 1);
			}
		}
	} else if(tipo == 2){
		for (int i = 0; i < numeroInimigos; i++) {
			if (i == 0) {
				inimigos[0] = { "General Zod", 40, {10, 18} };
			}
			else if (i == 1) {
				inimigos[1] = { "Metallo", 25, {6, 10} };
			}
			else if (i == 2) {
				inimigos[2] = { "Capanga", 10, {6, 10} };
			}
			else if (i == 3) {
				inimigos[3] = { "Starro", 20, {6, 10} };
			}
			else if (i == 4) {
				inimigos[4] = { "Lex Luthor", 25, {10, 15} };
			}
			else {
				inimigos[i].nome = "Pedra de Kryptonita" + std::to_string(i + 1);
			}
		}
	} else {
		return nullptr; // RETURN NULO, ALGUM ERRO OCORREU PRA CHEGAR AQUI!
	}

	return inimigos; // RESPONSABILIDADE DE QUEM CHAMOU PARA DESALOCAR ESSA MEMORIA!
}

Fase criarFase(int numeroInimigos, Inimigo* inimigos, int numeroFase, int alturaMapa, int larguraMapa) {
	Fase fase;

	fase.numeroInimigos = numeroInimigos;  // SETAR NA FASE O NÚMERO DE INIMIGOS!

	fase.inimigo = inimigos; // ARRAY DA FASE PARA APONTAR PARA O ARRAY DA FUNÇÃO INICALIZAR()!

	if (numeroFase == 1) {
		fase.nome = "Gotham City";
	}
	else if (numeroFase == 2) {
		fase.nome = "Metrópolis";
	}

	fase.mapa = criarMapa(alturaMapa, larguraMapa); // DECLARA A VARIÁVEL DO TIPO MAPA E CHAMA A FUNÇÃO PARA CRIAR A MATRIZ DO MAPA!

	for (int i = 0; i < alturaMapa; i++) { // LOOP PARA COLOCAR TODAS POSIÇÕES NÃO TER INIMIGOS!
		for (int j = 0; j < alturaMapa; j++) {
			fase.mapa.matriz[i][j].isInimigo = false;
		}
	}

	for (int iterador = 0; iterador < numeroInimigos; iterador++) {

		int i = rand() % alturaMapa; // SORTEIA O "i" PARA COLOCAR O INIMIGO!
		int j = rand() % larguraMapa; // SORTEIA O "j" PARA COLOCAR O INIMIGO!


		if (fase.mapa.matriz[i][j].isBloqueado == false && fase.mapa.matriz[i][j].isInimigo == false) { // COLOCA SE NÃO TIVER PEDRA NEM INIMIGO!
			fase.mapa.matriz[i][j].inimigoBloco = fase.inimigo; // FAZ A "MAGICA" ACONTECER!
			fase.inimigo++; // MUDA A POSIÇÃO DO PONTEIRO DE MEMORIA!
			fase.mapa.matriz[i][j].isInimigo = true; // SETA VERDADEIRO!
		}
		else if (fase.mapa.matriz[i][j].isBloqueado == false || fase.mapa.matriz[i][j].isInimigo == false) { // SE JÁ TIVE PEDRA OU INIMIGO VAI SORTEAR NOVAMENTE OU SE FOR A POSIÇAÕ INICIAL DO JOGADOR!
			iterador -= 1;
		}
	}
	fase.mapa.matriz[0][0].isBloqueado = false; // POSIÇÃO INCIAL NÃO PODE SER INIMIGO NEM PEDRA, ENTÃO NÃO VAI INIMIGO SE TIVER UMA PEDRA, AQUI SETO PRA NÃO SER PEDRA!

	return fase;
}

template <typename T1, typename T2>
void Atacar(T1& atacante, T2& atacado) {
	atacado.vida = atacado.vida - (atacante.arma.danoMinimo + (abs(rand()) % (atacante.arma.danoMaximo - atacante.arma.danoMinimo + 1))); // Calculo do dano
	cout << "\n O " << atacante.nome << " Atacou o " << atacado.nome << " Ficando com " << atacado.vida << " de Vida"; // Imprimir mensagem bonita
}

template <typename T>
bool taMorto(T morto) {
	if (morto.vida <= 0) {
		cout << "\n" << morto.nome << " Morreu!\n" << endl;
		return true;
	}
	return false;
}

Jogador combater(Fase fase, Jogador jogador) { // FUNÇÃO JOGAR FASE!
	bool jogadorMorto = false;
	bool inimigoMorto = false;

	while (jogadorMorto == false && inimigoMorto == false) { // ACABA SE UM DOS DOIS MORRER!
		Atacar(jogador, *fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].inimigoBloco); // CHAMA A FUNÇÃO E COLOCA COMO PARÂMETRO O JOGADOR E O BLOCO DO INIMIGO!
		Atacar(*fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].inimigoBloco, jogador); // CHAMA A FUNÇÃO E COLOCA COMO PARAMETRO O BLOCO DO INIMIGO E O JOGADOR
		inimigoMorto = taMorto(*fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].inimigoBloco);
		jogadorMorto = taMorto(jogador);
	}
	if (jogadorMorto) {
		cout << "\n Você Morreu!\n";
		return jogador;
	}
	fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].isInimigo = false;
	fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].inimigoBloco = nullptr; // REMOVE REFERENCIA DO INIMIGO DO BLOCO!
	cout << "\n Parabéns, Você Matou O Inimigo!\n";
	return jogador;

}

void movimentar(Fase fase, Jogador jogador) {
	while (jogador.vida > 0 && fase.numeroInimigos > 0) {
		cout << "Entrando em " << fase.nome << ", o(a) " << jogador.nome << " terá que derrorar todo os inimigos para sair ou irá morrer! \n";
		imprimirMapa(fase, jogador);

		cout << "\n Tecle W para ir para CIMA\n Tecle A para ir para ESQUERDA\n Tecle S para ir para BAIXO\n Tecle D para ir para DIREITA \n TECLA: ";
		char tecla = 0; // DECLARAÇÃO PARA UMA VARIÁVEL PARA PEGAR A TECLA!
		cin >> tecla; // PEGAR A LETRA DO TECLADO PARA FAZER A MOVIMENTAÇÃO!

		if (tecla == 'W' || tecla == 'w') {
			jogador.posicaoY--; // DIMINUI EM 1 O NUMERO DA LINHA, FAZENDO SUBIR!

		}
		else if (tecla == 'A' || tecla == 'a') { // AUMENTA EM 1 O NUMERO DA LINHA, FAZENDO DESCER!
			jogador.posicaoX--;

		}
		else if (tecla == 'S' || tecla == 's') { // DIMINUI EM 1 O NUMERO DA COLUNA, FAZENDO IR PARA ESQUERDA!
			jogador.posicaoY++;

		}
		else if (tecla == 'D' || tecla == 'd') { // AUMENTA EM 1 O NUMERO DA COLUNA, FAZENDO IR PARA DIREIRA!
			jogador.posicaoX++;

		}

		if (jogador.posicaoX < 0) { // CONDICIONAL PARA NÃO DEIXAR SAIR DO MAPA!
			cout << "\n Movimento Inválido!";
			jogador.posicaoX++; // AUMENTA A POSIÇÃO DA COLUNA PARA VOLTAR PRO MAPA!
		}
		else if (jogador.posicaoY < 0) {
			cout << "\n Movimento Inválido!";
			jogador.posicaoY++; // AUMENTA A POSIÇÃO DA LINHA PARA VOLTAR PARA O MAPA!
		}
		else if (jogador.posicaoX > fase.mapa.largura - 1) {
			cout << "\n Movimento Inválido!"; // DIMINUI A POSIÇÃO DA COLUNA PARA VOLTAR PARA O MAPA!
			jogador.posicaoX--;
		}
		else if (jogador.posicaoY > fase.mapa.altura - 1) {
			cout << "\n Movimento Inválido!";
			jogador.posicaoY--; // DIMINUI A POSIÇÃO DA LINHA PARA VOLTAR PRO MAPA!
		}

		cout << "\n ";

		if (fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].isBloqueado) {
			cout << "\n\n Não Pode";

			if (tecla == 'W' || tecla == 'w') {
				jogador.posicaoY++; // DIMINUI EM 1 O NUMERO DA LINHA, FAZENDO SUBIR!

			}
			else if (tecla == 'A' || tecla == 'a') { // AUMENTA EM 1 O NUMERO DA LINHA, FAZENDO DESCER!
				jogador.posicaoX++;

			}
			else if (tecla == 'S' || tecla == 's') { // DIMINUI EM 1 O NUMERO DA COLUNA, FAZENDO IR PARA ESQUERDA!
				jogador.posicaoY--;

			}
			else if (tecla == 'D' || tecla == 'd') { // AUMENTA EM 1 O NUMERO DA COLUNA, FAZENDO IR PARA DIREITA!
				jogador.posicaoX--;

			}
		}
		else if (fase.mapa.matriz[jogador.posicaoY][jogador.posicaoX].isInimigo) {
			jogador = combater(fase, jogador); // A VIDA DO JOGADOR RETORNAR 0, ACABA!
			fase.numeroInimigos--; // SE O JOGO NÃO ACABOU, ENTÃO ELE DERROTOU LOGO TERÁ QUE ENFRENTAR UM A MENOS!
		}
		else {
			cout << "\n Caminhou \n";
		}

		cout << "\n\n ";

		PAUSE("Pressione Enter Para Continuar...");
		CLEAR();
	}

	if (jogador.vida <= 0) {
		cout << "\n Você Morreu!!! \n ";
	}
	else {
		cout << "\n Parabéns, Você Venceu \n\n ";
	}
	PAUSE("Pressione Enter Para Continuar...");
}

void inicializador() {
	Jogador personagemJogador; // DECLARAÇÃO DA STRUCT DO JOGADOR!

	cout << "\n Escreva O Nome Do Seu Personagem: ";
	cin >> personagemJogador.nome; // NOME DO JOGADOR!

	int alturaMapa = 0; // DECLARAÇÃO DA ALTURA!
	int larguraMapa = 0; // DECLARAÇÃO DA LARGURA!

	cout << "\n Digite o Tamanho do Mapa ";

	while (alturaMapa <= 1) {
		cout << "\n Altura, tem que ser MAIOR que 1: ";
		cin >> alturaMapa; // INSERIR A ALTURA!
	}

	while (larguraMapa <= 1) {
		cout << "\n Largura, tem que ser MAIOR que 1: ";
		cin >> larguraMapa; // INSERIR A LARGURA!
	}

	int numeroInimigos = 0; // DECLARAÇÃO DA VARIÁVEL DO NUMERO DE INIMIGOS!

	while (numeroInimigos <= 0 || numeroInimigos > (alturaMapa * larguraMapa * 0.8)) { // 5 INIMIGOS PERSONALIZADOS!
		cout << "\n Digite O Número De Inimigos, Que Ocupe Até 80%(por Cento) Do Espaço Do Mapa: ";
		cin >> numeroInimigos; // INSERÇÃO DO NÚMERO DE INIMIGOS!
	}

	int numeroFase = 0;
	while (true) { // Looping para selecionar uma fase válida
		cout << "\n Escolha uma fase \n 1- Gotham City \n 2- Metrópolis \n Escolha:";
		cin >> numeroFase;
		if (numeroFase <= 2) {
			break;
		}
		cout << "\n Opção De Fase Inválida, Escolha Novamente!\n ";
		PAUSE("Pressione Enter Para Continuar...");
		CLEAR();
	}

	personagemJogador.vida = numeroInimigos * 25;

	CLEAR();
	Inimigo* inimigo = criarInimigos(numeroInimigos, numeroFase);
	auto fase = criarFase(numeroInimigos, inimigo, numeroFase, alturaMapa, larguraMapa); // DECLARAÇÃO E CRIAÇÃO DA FASE PELA FUNÇÃO!

	movimentar(fase, personagemJogador);

	for (int i = 0; i < fase.mapa.largura; i++) {
		delete[] fase.mapa.matriz[i];
	}
	delete[] fase.mapa.matriz;
	// DELETA a fase.inimigo;
	fase.mapa.matriz = nullptr;
}

int main() {
	srand(time(NULL));
	setlocale(LC_ALL, "Portuguese");

	int opcaoJogo = 0;
	bool is_running = true;
	while (is_running)
	{
		opcaoJogo = 0;
		cout << "Escolha uma opção:";
		cout << "\n 1- Jogar \n 2- Informações \n 3- Sair \n Opção: ";
		cin >> opcaoJogo;

		switch (opcaoJogo) { // SWITCH DA ESCOLHA DO QUE FAZER!
		case 1: // JOGO
			inicializador();
			break;
		case 2: // INFORMAÇÕES SOBRE O ALUNO!
			cout << "\n Informações:\n";
			cout << " Jogo feito por VALDINEI CARDOSO JUNIOR\n\n";
			cout << "\n INSPIRAÇÃO:\n";
			cout << "Para Criação Dos Cenários E Inimigos Eu Me Baseei Na DC (Universo De Quadrinhos DC Comics),\n";
			cout << "Trazendo Alguns Dos Personagens E Locais!\n\n";
			break;
		case 3: // SAIR DO JOGO
			cout << "\n Saiu do Jogo! \n";
			is_running = false;
			break;
		default: // OPÇÃO INVÁLIDA QUE PEDE UM NOVO DIGITO!
			cout << "\n Opção Inválida, Escolha Outra Opção!\n";
			break;
		}
	}
	return 0;
}
