#include "bingen.h"

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Modo de uso: %s <pontos.txt> <vizinhos.txt> <saida.bin>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Cria uma cópia do nome do arquivo de pontos.
    char points_fname[MAX_STR_LEN];
    strcpy(points_fname, argv[1]);

    // Cria uma cópia do nome do arquivo de vizinhos.
    char neighbours_fname[MAX_STR_LEN];
    strcpy(neighbours_fname, argv[2]);

    // Cria uma cópia do nome do arquivo de saída.
    char bin_fname[MAX_STR_LEN];
    strcpy(bin_fname, argv[3]);

    // Tenta abrir o arquivo de pontos para leitura.
    FILE *points_file = fopen(points_fname, "r");
    if (!points_file)
    {
        fprintf(stderr, "Ocorreu um erro ao tentar abrir o arquivo: %s", points_fname);
        exit(EXIT_FAILURE);
    }

    // Lê todos os Nodes do arquivo de pontos em um NodeList.
    NodeList *node_list = read_nodes_from_file(points_file);

    // Tenta abrir o arquivo de vizinhos para leitura.
    FILE *neighbours_file = fopen(neighbours_fname, "r");
    if (!neighbours_file)
    {
        fprintf(stderr, "Ocorreu um erro ao tentar abrir o arquivo: %s", neighbours_fname);
        free_node_list(node_list); // É necessário liberar qualquer memória alocada antes de encerrar.
        exit(EXIT_FAILURE);
    }

    // Lê do arquivo de vizinhos todas as arestas que conectam os nodes anteriormente lidos.
    read_edges_from_file(neighbours_file, node_list);

    // Tenta gerar o arquivo binário a partir da lista de Nodes e das arestas (Edges).
    if (!write_bin_data(node_list, bin_fname))
    {
        // Em caso de erro, aborta.
        free_node_list(node_list);
        fprintf(stderr, "Não foi possível gerar o arquivo binário: %s\n", bin_fname);
        exit(EXIT_FAILURE);
    }

    // Libera toda a memória alocada para os Nodes.
    free_node_list(node_list);

    // Fecha os arquivos.
    fclose(points_file);
    fclose(neighbours_file);

    return 0;
}

NodeList *read_nodes_from_file(FILE *points_file)
{
    // Aloca uma lista dinamicamente e inicia seu tamanho como 0.
    NodeList *list = (NodeList *)calloc(1, sizeof(NodeList));
    list->size = 0;

    // Buffer de leitura para o arquivo.
    char buf[MAX_STR_LEN];

    // Enquanto não chegar ao fim do arquivo
    while (fgets(buf, sizeof(buf), points_file))
    {
        // Aloca um novo Node dinamicamente.
        Node *node = (Node *)calloc(1, sizeof(Node));

        // Remove caracteres de CRLF do buffer.
        remove_crlf(buf);

        // Insere o vértice lido no novo Node.
        node->vertex = buf[0];

        // Lê as duas coordenadas x e y no novo Node.
        fscanf(points_file, "%f%f", &(node->x), &(node->y));

        // Limpa qualquer ruído remanescente no stream do arquivo.
        consume_input_garbage(points_file);

        // Lê as duas ruas componentes no novo Node.
        fgets(node->street_1, sizeof(node->street_1), points_file);
        fgets(node->street_2, sizeof(node->street_2), points_file);

        // Remove caracteres de CRLF de ambos os nomes das ruas.
        remove_crlf(node->street_1);
        remove_crlf(node->street_2);

        // Inicialza todas as arestas como NULL.
        node->north = NULL;
        node->south = NULL;
        node->east = NULL;
        node->west = NULL;

        // Adiciona o novo Node na frente da lista.
        node->next = list->head;
        list->head = node;
        list->size++;
    }

    return list;
}

void read_edges_from_file(FILE *neighbours_file, NodeList *list)
{
    // Buffer para leitura do arquivo.
    char buf[MAX_STR_LEN];

    // Enquanto não chegar ao fim do arquivo
    while (fgets(buf, sizeof(buf), neighbours_file))
    {
        // Cria variáveis para guardar informações dos vértices e nodes.
        char src_vertex, dst_vertex;
        Node *src_node, *dst_node;

        // Armazena o ponto do vértice de origem.
        remove_crlf(buf);
        src_vertex = buf[0];

        // Armazena o ponto do vértice de destino.
        fgets(buf, sizeof(buf), neighbours_file);
        remove_crlf(buf);
        dst_vertex = buf[0];

        // Extrai da lista de nodes os que são correspondentes aos vértices
        // de origem e destino, respectivamente.
        src_node = get_node_by_vertex(list, src_vertex);
        dst_node = get_node_by_vertex(list, dst_vertex);

        // Se ambos nodes existem
        if (src_node != NULL && dst_node != NULL)
        {
            // Aloca uma aresta dinamicamente.
            Edge *edge = (Edge *)calloc(1, sizeof(Edge));

            // Calcula as variações de posição entre os nodes de destino
            // e origem, respectivamente.
            float delta_x = dst_node->x - src_node->x;
            float delta_y = dst_node->y - src_node->y;

            // No mapa, a direção Norte é para baixo. Infere-se as demais
            // orientações

            // Variação positiva em x significa deslocamento para a direita (Oeste)
            if (delta_x > 0.0)
            {
                strcpy(edge->direction, "Oeste");
                src_node->east = edge;
            }
            // Variação negativa em x significa deslocamento para a esquerda (Leste)
            else if (delta_x < 0.0)
            {
                strcpy(edge->direction, "Leste");
                src_node->west = edge;
            }
            // Variação positiva em y significa deslocamento para cima (Sul)
            else if (delta_y > 0.0)
            {
                strcpy(edge->direction, "Sul");
                src_node->south = edge;
            }
            // Variação negativa significa deslocamento para baixo (Norte)
            else if (delta_y < 0.0)
            {
                strcpy(edge->direction, "Norte");
                src_node->north = edge;
            }

            // Salva o vértice de destino na nova aresta
            edge->dst = dst_vertex;

            // Calcula o peso (distância) entre os pontos
            edge->weight = sqrtf(pow(delta_x, 2) + pow(delta_y, 2));

            // Lê o nome da rua que compõe a aresta e conecta os dois nodes.
            fgets(edge->street, sizeof(edge->street), neighbours_file);
            remove_crlf(edge->street);
        }
    }
}

bool write_bin_data(NodeList *list, const char *filename)
{
    // Tenta abrir o arquivo para escrita em modo binário.
    FILE *outfile = fopen(filename, "wb");
    if (!outfile)
    {
        fprintf(stderr, "Erro ao abrir arquivo para escrita em modo binário: %s\n", filename);
        return false;
    }

    // Itera por todos os Nodes
    for (Node *node = list->head; node != NULL; node = node->next)
    {
        // Escreve os dados de cada node.
        fwrite(&(node->vertex), sizeof(node->vertex), 1, outfile);
        fwrite(&(node->x), sizeof(node->x), 1, outfile);
        fwrite(&(node->y), sizeof(node->y), 1, outfile);
        fwrite(node->street_1, sizeof(node->street_1), 1, outfile);
        fwrite(node->street_2, sizeof(node->street_2), 1, outfile);

        // Flags para indicar a presença ou não de arestas em determinada
        // direção.
        int north_flag = (node->north != NULL) ? 1 : 0;
        int south_flag = (node->south != NULL) ? 1 : 0;
        int east_flag = (node->east != NULL) ? 1 : 0;
        int west_flag = (node->west != NULL) ? 1 : 0;

        // Escreve as flags.
        fwrite(&north_flag, sizeof(int), 1, outfile);
        fwrite(&south_flag, sizeof(int), 1, outfile);
        fwrite(&east_flag, sizeof(int), 1, outfile);
        fwrite(&west_flag, sizeof(int), 1, outfile);

        // Escreve os dados das arestas, se presentes.
        if (north_flag)
            fwrite(node->north, sizeof(Edge), 1, outfile);
        if (south_flag)
            fwrite(node->south, sizeof(Edge), 1, outfile);
        if (east_flag)
            fwrite(node->east, sizeof(Edge), 1, outfile);
        if (west_flag)
            fwrite(node->west, sizeof(Edge), 1, outfile);
    }

    fclose(outfile);

    return true;
}