
#include "Prism.h"


typedef struct vertex {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;

} vertex;


D3D11_INPUT_ELEMENT_DESC  ied[] = {
	{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }

};


Prism::Prism(Graphics& gfx) :
	ObjModel(gfx)
{
}

Prism::~Prism()
{
	std::cout << "delete prism at :" << this << '\n';

};




GCLASS* GenerateCubeGCLASS(Graphics& gfx, int FaceNum) {

	float x, y, _x, _y, x_, y_;
	int j, numOfPoints = FaceNum * 2, numofVertecies = numOfPoints * 3;


	vertex* vBuffData = new vertex[numofVertecies];
	unsigned short* indeces = new unsigned short[FaceNum * 6 + (FaceNum - 2) * 6]{ 0 };
	
	for (int i = 0; i < FaceNum; ++i) {
		j = 6 * i;

		x = 0.5 * sin(MATH_PI * 2 * i / FaceNum);
		y = 0.5 * cos(MATH_PI * 2 * i / FaceNum);

		_x = 0.5 * sin(MATH_PI * 2 * (i - 0.5f) / FaceNum);
		_y = 0.5 * cos(MATH_PI * 2 * (i - 0.5f) / FaceNum);

		x_ = 0.5 * sin(MATH_PI * 2 * (i + 0.5f) / FaceNum);
		y_ = 0.5 * cos(MATH_PI * 2 * (i + 0.5f) / FaceNum);


		vBuffData[j] = { {x  ,y  ,0.5    },  {0.0f,0.0f,1.0f   } };
		vBuffData[j + 1] = { {x  ,y  ,0.5    },  {_x  ,_y  ,0.0f   } };
		vBuffData[j + 2] = { {x  ,y  ,0.5    },  {x_  ,y_  ,0.0f   } };
		vBuffData[j + 3] = { {x  ,y  ,-0.5   },  {0.0f, 0.0f, -1.0f} };
		vBuffData[j + 4] = { {x  ,y  ,-0.5   },  {_x  ,_y  ,0.0f   } };
		vBuffData[j + 5] = { {x  ,y  ,-0.5   },  {x_  ,y_  ,0.0f   } };

	}
	/*
	for (int i = 0; i < numofVertecies; ++i)
	{
		printf("[possition] = < %f , %f, %f >\t[normals] = < %f , %f, %f >\n",
			vBuffData[i].pos.x,
			vBuffData[i].pos.y,
			vBuffData[i].pos.z,
			vBuffData[i].normal.x,
			vBuffData[i].normal.y,
			vBuffData[i].normal.z
		);
	}

	*/
	for (int i = 0; i < FaceNum * 2; i += 2) {
		j = i * 3;
		indeces[j] = 3 * (i)+3;
		indeces[j + 1] = (3 * (i + 2) + 2)%numofVertecies;
		indeces[j + 2] = (3 * (i + 1) + 3)%numofVertecies;
		indeces[j + 3] = (3 * (i + 2) + 2)%numofVertecies;
		indeces[j + 4] = (3 * (i + 1) + 3)%numofVertecies;
		indeces[j + 5] = (3 * (i + 3) + 2)%numofVertecies;

	}
	for (int i = 0; i < FaceNum * 6; i += 3) {
		printf("%hu %hu %hu \n", indeces[i], indeces[i + 1], indeces[i + 2]);

	}
	
	

	/*
	for (int i = 0; i < FaceNum; ++i) {
		j = 6 * i;
		indeces[j]		= 3 * (2 * i)	+3;
		indeces[j + 1]	= 3 * (2 * i + 1) % numOfPoints +3 ;
		indeces[j + 2]	= 3 * (2 * i + 3) % numOfPoints +2;
		//				 	 *3						
		indeces[j + 3]	= 3 * (2 * i)+2;
		indeces[j + 5]	= 3 * (2 * i + 3) % numOfPoints+3;
		indeces[j + 4]	= 3 * (2 * i + 2) % numOfPoints+2;

	}
	*/
	j = FaceNum * 6;
	for (int i = 1; i < FaceNum - 1; ++i) {
		indeces[j] = 0;
		indeces[j + 1] = (2 * i + 2) % numOfPoints;
		indeces[j + 2] = (2 * i) % numOfPoints;
		j += 3;
	}

	j = FaceNum * 6 + (FaceNum - 2) * 3;
	for (int i = 1; i < FaceNum - 1; ++i) {
		indeces[j] = 1;
		indeces[j + 1] = (2 * i + 1) % numOfPoints;
		indeces[j + 2] = (2 * i + 3) % numOfPoints;
		j += 3;
	}


	char str[50] = { 0 };
	sprintf(str, "Prism%d", FaceNum);
	GCLASS* pgclass = new GCLASS(str);

	VertexShader* vs = new VertexShader(gfx, L"AssVertexShader.cso");
	pgclass->AddBindable(new IndexBuffer(gfx, indeces, FaceNum * 6 + (FaceNum - 2) * 6));
	pgclass->AddBindable(new VertexBuffer(gfx, vBuffData, FaceNum * 2 * 3));

	pgclass->AddBindable(vs);
	pgclass->AddBindable(new PixelShader(gfx, L"AssPixelShader.cso"));

	pgclass->AddBindable(new InputLayout(gfx, ied, 2u, vs->getpBlob()));

	pgclass->AddBindable(new TransformCBuffer(gfx));

	pgclass->AddBindable(new PrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	return pgclass;
}

