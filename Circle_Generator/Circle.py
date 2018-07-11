from PIL import Image, ImageDraw

image = Image.new('RGBA', (512, 512))
draw = ImageDraw.Draw(image)
draw.rectangle((0,0,512,512),  fill =(0,0,0,0), outline =(0,0,0,0))
draw.ellipse((95,195,225,325), fill =(1,1,1,1), outline =(1,1,1,1))

image.save('circulo4.bmp')	
