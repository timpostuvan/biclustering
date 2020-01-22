from skimage import io
from skimage import img_as_float
from skimage.color import rgb2gray

def output_image_matrix(image_matrix_path, image_float):
	f = open(image_matrix_path, "w+")
	n = len(image_float)
	m = len(image_float[0])


	f.write(str(n) + " " + str(m) + "\n")
	for i in range(n):
		for j in range(m):
			f.write(str(image_float[i][j]))
			if(j != m - 1):
				f.write(" ")

		f.write("\n")

	f.close()



image_path = "images-data/flower.jpg"
#image_path = "images-data/caribbean.jpg"

image_matrix_path = image_path.rsplit(".", 1)[0] + "_matrix.txt"


image = io.imread(image_path)

io.imshow(image)
io.show()

gray_image = rgb2gray(image)

io.imshow(gray_image)
io.show()

image_float = img_as_float(gray_image)
output_image_matrix(image_matrix_path, image_float)

print(image_float)