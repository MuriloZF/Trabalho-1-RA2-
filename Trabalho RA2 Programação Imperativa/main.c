#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Carro {
    char marca[20];
    char modelo[20];
    int ano;
    int km;
    float preco;
} Carro;

// FUNÇÕES

void exibirCarros(FILE *file);
void exibirMarca(FILE *file, const char *marca);
void exibirRange(FILE *file, float preco_min, float preco_max);
void inserirCarro(FILE *file);
void kmMax(FILE *file, int km_max);

int main() {
    FILE * file;
    file = fopen("carros.txt", "a+"); // ABRE O ARQUIVO;

    if(file == NULL) {
        printf("Falha ao abrir o arquivo .txt!");
        exibirCarros(file);
    }else {
        int escolha;
        do { // DO WHILE PARA MENU
            printf("1-Exibir todos os carros\n"
               "2-Exibir todos os carros de uma marca\n"
               "3-Exibir todos os carros dentro de um intervalo de precos\n"
               "4-Inserir um novo carro\n"
               "5-Remover carros superiores a uma kilometragem\n"
               "6-Sair\n"
               "Escolha: ");
            scanf("%d", &escolha);

            switch (escolha) {
                case 1:
                    printf("Lista de todos os carros:\n");
                    exibirCarros(file);
                break;
                case 2: {
                    char marca[30];
                    printf("Digite o nome da marca: ");
                    scanf("%s", marca);
                    printf("Lista de carros da %s:\n", marca);
                    exibirMarca(file, marca);
                    break;
                }
                case 3: {
                    float preco_min;
                    float preco_max;
                    printf("Digite o preco minimo: ");
                    scanf("%f", &preco_min);
                    printf("\nDigite o preco maximo: ");
                    scanf("%f", &preco_max);
                    printf("Lista de carros no intervalo de R$:%.2f a R$:%.2f\n", preco_min, preco_max);
                    exibirRange(file, preco_min, preco_max);
                    break;
                }
                case 4: {
                    printf("Inserir um novo carro: \n");
                    inserirCarro(file);
                    break;
                }
                case 5: {
                    int km_max;
                    printf("Digite a kilometragem maxima: ");
                    scanf("%d", &km_max);
                    kmMax(file, km_max);
                    printf("Carros com a kilometragem superior a %d foram removidos\n", km_max);
                    break;
                }
                case 6: {
                    exit(0);
                }
                default: {
                    printf("Opcao invalida! O programa sera encerrado \n");
                    exit(0);
                }
            }
        } while(escolha != 6);
    }
    fclose(file);
    return 0;
}
// FUNÇÃO PARA EXIBIR TODOS OS CARROS
void exibirCarros(FILE *file) {
    Carro carro;
    rewind(file); // MOVE O PONTEIRO PARA O COMEÇO!!!

    while (fscanf(file, "%19[^\n]\n%19[^\n]\n%d\n%d\n%f\n",
                  carro.marca, carro.modelo, &carro.ano, &carro.km, &carro.preco) == 5) {
        printf("Marca: %s | Modelo: %s | Ano: %d | Kilometragem: %d | Preco: %.2f\n",
               carro.marca, carro.modelo, carro.ano, carro.km, carro.preco);
                  }
}
 // FUNÇÃO PARA EXIBIR TODOS OS CARROS DE UMA MARCA
void exibirMarca(FILE *file, const char * marca) {
    Carro carro;
    rewind(file);
    while (fscanf(file, "%19[^\n]\n%19[^\n]\n%d\n%d\n%f\n",
                  carro.marca, carro.modelo, &carro.ano, &carro.km, &carro.preco) == 5) {
        if (strcmp(carro.marca, marca) == 0) { // COMPARA SE A MARCA DO CARRO É IGUAL A marca
            printf("Marca: %s | Modelo: %s | Ano: %d | Kilometragem: %d | Preco: %.2f\n",
               carro.marca, carro.modelo, carro.ano, carro.km, carro.preco);
        }
    }
}
// FUNÇÃO PARA FILTRAR POR RANGE DE PREÇO
void exibirRange(FILE *file, float preco_min, float preco_max) {
    Carro carro;
    rewind(file);
    while (fscanf(file, "%19[^\n]\n%19[^\n]\n%d\n%d\n%f\n",
                  carro.marca, carro.modelo, &carro.ano, &carro.km, &carro.preco) == 5) {
        if (carro.preco >= preco_min && carro.preco <= preco_max ) {
            printf("Marca: %s | Modelo: %s | Ano: %d | Kilometragem: %d | Preco: %.2f\n",
              carro.marca, carro.modelo, carro.ano, carro.km, carro.preco);
        }
    }
}
// FUNÇÃO PARA ADICIONAR NOVO CARRO
void inserirCarro(FILE *file) {
    Carro carro;
    fseek(file, 0, SEEK_END);
    printf("Marca:");
    scanf("%s", &carro.marca);
    printf("\nModelo: ");
    scanf("%s", &carro.modelo);
    printf("\nAno: ");
    scanf("%d", &carro.ano);
    printf("\nKilometragem: ");
    scanf("%d", &carro.km);
    printf("\nPreco: ");
    scanf("%f", &carro.preco);
    fprintf(file, "%s\n%s\n%d\n%d\n%.2f", carro.marca, carro.modelo, carro.ano, carro.km, carro.preco);

}

void kmMax(FILE *file, int km_max) {
    FILE *carrosNOVO = fopen("carrosNOVO.txt", "w"); // CRIAÇÃO ARQUIVO NOVO!!!
    if(carrosNOVO == NULL) {
        printf("Erro ao criar o novo arquivo.");
        exit(0);
    }else {
        Carro carro;
        while (fscanf(file, "%19[^\n]\n%19[^\n]\n%d\n%d\n%f\n",
                  carro.marca, carro.modelo, &carro.ano, &carro.km, &carro.preco) == 5) {
            if(carro.km <= km_max) {
                fprintf(carrosNOVO, "%s\n%s\n%d\n%d\n%.2f\n",
                        carro.marca, carro.modelo, carro.ano, carro.km, carro.preco);
            }
                  }
    }
    // SUBSTITUIÇÃO DOS ARQUIVOS!!!
    fclose(file);
    fclose(carrosNOVO);
    remove("carros.txt");
    rename("carrosNOVO.txt", "carros.txt");
    fopen("carros.txt", "a+");
}