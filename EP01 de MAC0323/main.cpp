#include <cstdlib>
#include <string>
#include <random>
#include <iostream>
#include <utility>
#include <deque>


class Aviao {
    public:
        bool isPouso;
        bool isEmergencia;
        std::string idAviao;
        std::string destino;
        int combustivel;
        int tempo_de_voo;
        int tempo_na_fila;
        Aviao (bool isPouso, bool isEmergencia, std::string idAviao, std::string destino, int combustivel, int tempo_de_voo, int tempo_na_fila) {
            this->isPouso = isPouso;
            this->isEmergencia = isEmergencia;
            this->idAviao = std::move(idAviao);
            this->destino = std::move(destino);
            this->combustivel = combustivel;
            this->tempo_de_voo = tempo_de_voo;
            this->tempo_na_fila = tempo_na_fila;
        }
        void print() const {
            std::cout << "\nAviao " << this->idAviao << " - ";
            if (this->isPouso) {
                std::cout << "Pouso - ";
            } else {
                std::cout << "Decolagem - " << "Tempo de voo: " << this->tempo_de_voo << std::endl;
            }
            if (this->isEmergencia) {
                std::cout << "Emergencia - ";
            }
            std::cout << "Destino: " << this->destino << " - Combustivel: " << this->combustivel << " - Tempo na fila: "
            << this->tempo_na_fila << std::endl;
        }
    };

using namespace std;

//Cria um avião com atributos aleatórios utilizando os parâmetros entrados pelo usuário
Aviao cria_aviao(float pp, float pe, int C, int V){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> combustivel_dist(3, C);
    std::uniform_int_distribution<int> tempo_de_voo_dist(3, V);
    std::uniform_int_distribution<int> letter_dist('A', 'Z');
    std::uniform_int_distribution<int> number_dist(0, 9);
    std::uniform_real_distribution<float> pouso_dist(0, 10000);
    std::uniform_real_distribution<float> emergencia_dist(0, 10000);
    bool pouso = pouso_dist(gen) < (pp * 10000);
    bool emergencia = emergencia_dist(gen) < (pe * 10000);
    int combustivel = combustivel_dist(gen);
    int tempo_de_voo = tempo_de_voo_dist(gen);
    //gera letras e números para a identificação do voo
    char letra1 = static_cast<char>(letter_dist(gen));
    char letra2 = static_cast<char>(letter_dist(gen));
    int numero1 = number_dist(gen);
    int numero2 = number_dist(gen);
    int numero3 = number_dist(gen);
    std::string idAviao;
    idAviao += letra1;
    idAviao += letra2;
    idAviao += std::to_string(numero1);
    idAviao += std::to_string(numero2);
    idAviao += std::to_string(numero3);
    //gera letras para a identificação do destino do voo
    char letra3 = static_cast<char>(letter_dist(gen));
    char letra4 = static_cast<char>(letter_dist(gen));
    char letra5 = static_cast<char>(letter_dist(gen));
    std::string destino;
    destino += letra3;
    destino += letra4;
    destino += letra5;
    return {pouso, emergencia, idAviao, destino, combustivel, tempo_de_voo, 0};
}

int main(){
    std::deque<Aviao*> fila_normal; //fila carregando os ponteiros para os aviões
    std::deque<Aviao*> fila_emergencial;
    std::deque<Aviao> banco_de_avioes; //banco de dados que carregará todos os aviões por ordem de chegada
    int max_avioes;
    int tp = 0; //Tempo médio de pouso
    int td = 0; //Tempo médio de decolagem
    int cp = 0; //Combustível dos aviões que pousaram
    int ce = 0; //Combustível médio dos aviões esperando para pousar
    int ap = 0; //Quantidade de aviões que pousaram
    int pde; //Quantidade de aviões pousando/decolando em condições de emergência
    //Pistas e seus contadores para manutenção
    Aviao *pista1;
    Aviao *pista2;
    Aviao *pista3;
    pista1 = nullptr; pista2 = nullptr; pista3 = nullptr;
    int cont_pista1 = 0;
    int cont_pista2 = 0;
    int cont_pista3 = 0;
    std::srand(13696262);
    //Entrada dos parâmetros
    int T; //Unidade de tempo de simulação
    int K; //Número máximo de aviões que chegam no aeroporto em cada unidade de tempo
    float pp; //probabilidade de ser um pouso
    float pd; //probabilidade de ser uma decolagem (1.0 - pp)
    float pe; //probabilidade de ser emergência
    int C; //tempo máximo de combustível de um avião que deseja pousar
    int V; //tempo máximo de voo de uma decolagem
    cout << "Bem-vindo ao simulador de gestao de avioes de um aeroporto (EP0 - MAC0323) Matheus Oliveira da Silva\n\n";
    cout << "Insira a unidade de tempo: ";
    cin >> T;
    cout << "Insira o numero maximo de avioes que chegam no aeroporto em cada unidade de tempo: ";
    cin >> K;
    cout << "Insira a probabilidade de ser um pouso (em decimal): ";
    cin >> pp;
    pd = 1 - pp;
    cout << "A probabilidade de ser uma decolagem sera (em decimal): " << pd;
    cout << "\nInsira a probabilidade de ser uma emergencia (em decimal): ";
    cin >> pe;
    cout << "Insira o tempo maximo de combustivel de um aviao que deseja pousar: ";
    cin >> C;
    cout << "Insira o tempo maximo de voo de uma decolagem: ";
    cin >> V;
    cout << "\n\nSua simulacao sera feita com os seguintes parametros:\n" << "Unidade de tempo: " << T;
    cout << "\nNumero maximo de avioes por unidade de tempo: " << K;
    cout << "\nProbabilidade de ser um pouso: " << pp;
    cout << "\nProbabilidade de ser uma decolagem: " << pd;
    cout << "\nProbabilidade de ser uma emergencia: " << pe;
    cout << "\nTempo maximo de combustivel: " << C;
    cout << "\nTempo maximo de voo: " << V << endl;
    for (int i = 0; i < T; ++i) {
        cout << "Iteracao: " << i + 1 << endl;
        pde = 0;
        //Se a pista esteve ocupada durante 1 rodada considera o avião decolado/pousado
        if (cont_pista1 == 1) {
            if (pista1->isPouso) {
                ap += 1;
                cp += pista1->combustivel;
            }
        }
        if (cont_pista2 == 1) {
            if (pista2->isPouso) {
                ap += 1;
                cp += pista2->combustivel;
            }
        }
        if (cont_pista3 == 1) {
            if (pista3->isPouso) {
                ap +=1;
                cp += pista3->combustivel;
            }
        }
        if (K != 0) max_avioes = rand() % K;
        else max_avioes = 0;
        //Gera até K aviões em cada unidade de tempo
        for (int j = 0; j < max_avioes; ++j) {
            Aviao aux = cria_aviao(pp, pe, C, V);
            banco_de_avioes.push_back(aux);
            //Como não há critério de qual emergência é mais prioritária, optei por atender as emergências por ordem de
            //chegada
            if (aux.isEmergencia) {
                fila_emergencial.push_back(&banco_de_avioes.back());
            }
            else fila_normal.push_back(&banco_de_avioes.back());
        }

        /*Condições para prioridade de pouso e decolagem:
          Um pouso é considerado emergencial se ele foi gerado de forma emergencial ou se chegou em um dos seguintes
          critérios:
          i) é um pouso que ficará sem combustível;
          ii) é uma decolagem que está em espera há mais de 10% do seu tempo de voo;
          */
        for (int j = 0; j < fila_normal.size(); ++j) {
            if (fila_normal[j]->isPouso) {
                if (fila_normal[j]->combustivel < 5) {
                    fila_emergencial.push_front(fila_normal[j]);
                    fila_normal.erase(fila_normal.begin() + j);
                }
            }
            else {
                if (fila_normal[j]->tempo_na_fila >= 0.1 * fila_normal[j]->tempo_de_voo) {
                    fila_emergencial.push_back(fila_normal[j]);
                    fila_normal.erase(fila_normal.begin() + j);
                }
            }
        }

        //Insere aviões na pista se estão vazias
        if (pista1 == nullptr) {
            if (!fila_emergencial.empty()) {
                pista1 = fila_emergencial.front();
                fila_emergencial.pop_front();
            } else if (!fila_normal.empty()) {
                pista1 = fila_normal.front();
                fila_normal.pop_front();
            }
        }

        if (pista2 == nullptr) {
            if (!fila_emergencial.empty()) {
                pista2 = fila_emergencial.front();
                fila_emergencial.pop_front();
            } else if (!fila_normal.empty()) {
                pista2 = fila_normal.front();
                fila_normal.pop_front();
            }
        }

        //pista 3 só é utilizada por aviões emergenciais ou aviões de decolagem
        if (pista3 == nullptr) {
            if (!fila_emergencial.empty()) {
                pista3 = fila_emergencial.front();
                fila_emergencial.pop_front();
            }
            else if (!fila_normal.empty()){
                for (int j = 0; j < fila_normal.size(); ++j) {
                    if (!fila_normal[j]->isPouso) {
                        pista3 = fila_normal[j];
                        fila_normal.erase(fila_normal.begin() + j);
                        break;
                    }
                }
            }
        }

        /*Altera todas as variáveis relativas à unidade de tempo (combustível, tempo de espera, ...)
         * Aqui eu considero que o atributo combustível é irrelevante para decolagens e o atributo tempo na fila é ir-
         * relevante para pousos, visto que o combustível restante é mais importante
         */
        for (int j = 0; j < fila_emergencial.size(); ++j) {
            if (fila_emergencial[j]->isPouso) {
                fila_emergencial[j]->combustivel += -1;
                tp += 1;//número de aviões que querem pousar
                ce += fila_emergencial[j]->combustivel;
                //desvia para outro aeroporto caso o avião fique sem combustível
                if (fila_emergencial[j]->combustivel <= 1) fila_emergencial.erase(fila_emergencial.begin() + j);
            }
            else {
                fila_emergencial[j]->tempo_na_fila += 1;
                td += 1;
            }
        }
        for (int j = 0; j < fila_normal.size(); ++j) {
            if (fila_normal[j]->isPouso) {
                fila_normal[j]->combustivel += -1;
                tp += 1;//número de aviões que querem pousar
                ce += fila_normal[j]->combustivel;
                //desvia para outro aeroporto caso o avião fique sem combustível
                if (fila_normal[j]->combustivel <= 1) fila_normal.erase(fila_normal.begin() + j);
            }
            else {
                fila_normal[j]->tempo_na_fila += 1;
                td += 1;
            }
        }

        cout << "Quantidade de avioes contatados: " << max_avioes << endl;
        cout << "Quantidade de avioes na fila normal: " << fila_normal.size() << endl;
        cout << "Quantidade de avioes na fila emergencial: " << fila_emergencial.size() << endl;
        cout << "Quantidade de avioes pousando/decolando em situacao de emergencia: " << pde << endl;
        //Impressão dos aviões das filas de espera
        cout << "Avioes na fila de espera normal: " << endl;
        if (!fila_normal.empty()) for (auto & j : fila_normal) j->print();
        else cout << "fila vazia" << endl;
        if (!fila_emergencial.empty()) cout << "Avioes na fila de espera emergencial: " << endl;
        else cout << "fila vazia" << endl;
        for (auto & j : fila_emergencial) j->print();

        cout << "\nTempo medio de pouso: " << tp / 2 << endl;
        cout << "Tempo medio de decolagem: " << td / 3 << endl;
        if (tp != 0) cout << "Combustivel medio dos avioes esperando para pousar: " << ce/tp + 1 << endl; /*tp também
        * equivale ao número de aviões querendo pousar*/
        else cout << "Combustivel medio dos avioes esperando para pousar: " << 0 << endl;
        if (ap != 0) cout << "Combustivel medio dos avioes que pousaram: " << cp/ap << endl;
        else cout << "Combustivel medio dos avioes que pousaram: " << 0 << endl;
        cout << "Situacao das pistas: " << endl;
        if (pista1 != nullptr) {
            if (pista1->isEmergencia) pde+=1;
            cont_pista1 += 1;
            cout << "Pista 1 ocupada" << endl;
        }
        else cout << "Pista 1 desocupada" << endl;
        if (pista2 != nullptr) {
            cont_pista2 += 1;
            if (pista2->isEmergencia) pde+=1;
            cout << "Pista 2 ocupada" << endl;
        }
        else cout << "Pista 2 desocupada" << endl;
        if (pista3 != nullptr) {
            cont_pista3 += 1;
            if (pista3->isEmergencia) pde+=1;
            cout << "Pista 3 ocupada" << endl;
        }
        else cout << "Pista 3 desocupada" << endl;
        tp = 0;
        td = 0;
        ce = 0;
        cout << "_______________________________" << endl;
        //Se a pista esteve ocupada durante 3 rodadas, sendo 1 de pouso/decolagem e 2 de manutenção, libera a pista
        if (cont_pista1 == 3) {
            pista1 = nullptr;
            cont_pista1 = 0;
        }
        if (cont_pista2 == 3) {
            pista2 = nullptr;
            cont_pista2 = 0;
        }
        if (cont_pista3 == 3) {
            pista3 = nullptr;
            cont_pista3 = 0;
        }
    }
    return 0;
}
