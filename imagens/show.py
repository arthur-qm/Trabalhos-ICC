"""
Esse programa converte um arquivo com os valores grayscale dos pixels na forma
de matriz para uma imagem
"""

import sys

def main(nome=''):
    from PIL import Image as im
    import numpy as np
    
    if not nome:
        nome = input('Diga o nome do arquivo: ') 
    
    converter = lambda x: int(float(x) * 255)
    
    vals = np.transpose(np.loadtxt(nome, skiprows=1))
    vals *= 255
    vals = vals.astype(np.uint8)
    
    print(vals)

    dados = im.fromarray(vals)
    dados.show()

if __name__ == '__main__':
    main(*sys.argv[1:])

