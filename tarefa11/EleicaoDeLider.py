import random
import time

class Processo:
    def __init__(self, id):
        self.id = id
        self.ativo = True
        self.heartbeat_interval = 1
        self.encarnacao = 1

    def falhar(self):
        self.ativo = False
        print(f"[INFO] Processo {self.id} falhou.")

    def recuperar(self):
        self.ativo = True
        self.encarnacao += 1
        print(f"[INFO] Processo {self.id} se recuperou com encarnação {self.encarnacao}.")

class SistemaDistribuido:
    def __init__(self, num_processos):
        self.processos = [Processo(i) for i in range(num_processos)]
        self.lider = None

    def eleger_lider(self):
        candidatos = [p for p in self.processos if p.ativo]
        if candidatos:
            # Critério: escolher processo com menor ID (equivalente ao que falhou menos vezes)
            self.lider = min(candidatos, key=lambda p: p.encarnacao)
            print(f"[ELEIÇÃO] Novo líder eleito: Processo {self.lider.id} (encarnação {self.lider.encarnacao})")
        else:
            self.lider = None
            print("[ELEIÇÃO] Não há líderes disponíveis, todos os processos falharam.")

    def executar_consenso(self):
        valores_propostos = []
        for p in self.processos:
            if p.ativo:
                valor = random.randint(0, 1)  # Cada processo propõe um valor (0 ou 1)
                valores_propostos.append(valor)
                print(f"[CONSENSO] Processo {p.id} propôs o valor {valor}.")
        if valores_propostos:
            valor_consenso = max(set(valores_propostos), key=valores_propostos.count)
            print(f"[CONSENSO] Valor decidido pelo consenso: {valor_consenso}\n")

    def simular(self):
        # Inicialmente, eleger um líder
        self.eleger_lider()
        self.executar_consenso()

        # Simular falha de um processo aleatório
        falha_id = random.randint(0, len(self.processos) - 1)
        print(f"[SIMULAÇÃO] Simulando falha no Processo {falha_id}...")
        self.processos[falha_id].falhar()
        self.eleger_lider()
        self.executar_consenso()

        # Recuperar processo falho
        print(f"[SIMULAÇÃO] Recuperando Processo {falha_id}...\n")
        self.processos[falha_id].recuperar()
        self.eleger_lider()
        self.executar_consenso()

# Simular o sistema distribuído com X processos
X = 5
sistema = SistemaDistribuido(X)
sistema.simular()
