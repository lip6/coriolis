import os

def remove_lines_with_pwell(directory):
    # Parcourt tous les fichiers dans le répertoire courant
    for filename in os.listdir(directory):
        # Vérifie si le fichier a l'extension .ap
        if filename.endswith('.ap'):
            # Lis le fichier et stocke toutes les lignes sauf celles contenant 'PWELL'
            with open(filename, 'r') as file:
                lines = file.readlines()
            
            with open(filename, 'w') as file:
                for line in lines:
                    if 'PWELL' not in line:
                        file.write(line)
remove_lines_with_pwell('./')          
