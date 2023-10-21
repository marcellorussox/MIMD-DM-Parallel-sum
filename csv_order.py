import pandas as pd

# Inserisci il percorso del tuo file CSV
csv_path = 'output_sorted.csv'

# Carica il CSV in un DataFrame
df = pd.read_csv(csv_path)

# Ordina il DataFrame in base alle colonne 1, 2 e 3 in ordine crescente
df_sorted = df.sort_values(by=[df.columns[0], df.columns[1], df.columns[2]])

# Salva il DataFrame ordinato in un nuovo file CSV
csv_sorted_path = 'output_sorted.csv'
df_sorted.to_csv(csv_sorted_path, index=False)

print(f'File CSV ordinato salvato in: {csv_sorted_path}')