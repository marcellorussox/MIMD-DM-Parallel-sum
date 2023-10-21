import pandas as pd

def riduzione_media_csv(file_path):
    # Carica il file CSV in un DataFrame senza nomi di colonna
    try:
        df = pd.read_csv(file_path, header=None)
    except FileNotFoundError:
        print(f"File non trovato: {file_path}")
        return

    # Qui, utilizziamo gli indici delle colonne (0, 1, 2) per il raggruppamento e il calcolo della media
    grouped_data = df.groupby([0, 1, 2]).mean()

    # Stampa i risultati
    print("Risultati della riduzione tramite media:")
    print(grouped_data)

if __name__ == "__main__":
    # Passa il percorso del file CSV come argomento
    # file_path = input("Inserisci il percorso del file CSV: ")
    riduzione_media_csv('output_sorted.csv')
