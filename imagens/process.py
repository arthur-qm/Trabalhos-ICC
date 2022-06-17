import sys

def main(original='', cinza_nome=''):
    from PIL import Image, ImageOps
    import numpy as np
    
    if not original:
        original = input('Qual e o arquivo que quer botar em grayscale? ')
    
    og_img = Image.open(original)
    w, h = og_img.size
    
    if not cinza_nome:
        cinza_nome = input('Qual vai ser o nome do arquivo com a imagem cinza? ')
    
    gr_img = ImageOps.grayscale(og_img)

    px = np.array(list(gr_img.getdata())).reshape((h, w))
    np.savetxt(cinza_nome, np.transpose(px/255), header=f'{w} {h}', fmt='%.2f', comments='')
    


if __name__ == '__main__':
    main(*sys.argv[1:])

