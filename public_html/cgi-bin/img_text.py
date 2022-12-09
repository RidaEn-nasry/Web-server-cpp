import pytesseract as tess
from PIL import Image, ImageEnhance, ImageFilter
import sys

def func(value):
    return ' '.join(value.splitlines())

# Path to tesseract executable

img = Image.open(sys.argv[1])
text = tess.image_to_string(img)

print('<!DOCTYPE html> <html lang="en"> <head><meta charset="UTF-8">')
print('	<title>File Upload UI Design</title><link rel="stylesheet" href="../img_convertor/style_convertor.css">')
print('</head> <body> <div class="wrapper"><div class="title">IMAGE CONVERTOR</div><div class="file_upload_list"></div>')
print('<div class="choose_file"><label for="choose_file"><input type="file" id="choose_file"><span>Choose File</span>')
print('</label></label><section class="input-content" ><h2>Text Output</span></h2>')
print('<textarea>')
print(func(text))
print('</textarea>')

print('</section></div></div></body></html>')