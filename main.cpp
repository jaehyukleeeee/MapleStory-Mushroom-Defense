//-----------------------------------------------------------------------------
// File: ss.cpp
//
// Desc: This is the first tutorial for using Direct3D. In this tutorial, all
//       we are doing is creating a Direct3D device and using it to clear the
//       window.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )




//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9         g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9   g_pd3dDevice = NULL; // Our rendering device
LPD3DXSPRITE g_pSprite;

struct Image {
	LPDIRECT3DTEXTURE9 texture;
	BOOL visible;
	RECT rect;
	D3DXVECTOR3 center;
	D3DXVECTOR3 pos;
	//-----------------------------------------------------------------------------
	INT frame;
	INT Iframe;//유휴상태
	INT Aframe;//공격상태
	INT Wframe;//걷는중
	INT Hframe;//피격상태
	//-----------------------------------------------------------------------------
	BOOL IdleState;
	BOOL AttackState;
	BOOL WalkingState;
	BOOL HitState;
	BOOL DieState;
	//-----------------------------------------------------------------------------
	INT rx;//스킬값
	INT rrx;//스킬값2
	INT lx;
	INT llx;
	INT wx;//걷는값
	INT wwx;//걷는값2
	INT dx;//죽는값
	INT ddx;//죽는값2
	INT HP;
	//-----------------------------------------------------------------------------
	BOOL L;
	BOOL R;
};

enum GAME_STATE {
	INIT, READY, RUNNING, STOP, SUCCESS, FAILED, RESULT
};

Image init;
Image tur1;
Image tur2;
Image wave1;
Image wave2;
Image wave3;
Image clear;
Image fail;
Image map;
Image testcoll;
Image test;
Image test2;
Image player;
Image arrow[100];
Image monster[10];
Image SpawnManager;
Image hp;
Image clearmsg;
Image failmsg;

INT delay = 0;
INT s = 0;
INT deathcnt = 0;
INT killcnt = 0;
INT wave = 1;
INT wavedelay = 0;
INT sdelay = 0;

BOOL wavectrl = TRUE;

GAME_STATE g_GameState = INIT;
clock_t g_OldTime;



VOID Init();
VOID Update();
VOID Sframe();
VOID PIdle();
VOID Arrow();

VOID MIdle();
VOID MDie();
//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = FALSE; // 전체화면모드로 생성
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // 가장 효율적인 SWAP효과
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; // 현재 바탕화면 모드에 맞춰 백버퍼를 생성
	d3dpp.BackBufferWidth = 800;
	d3dpp.BackBufferHeight = 600;

    //d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	

    // Create the Direct3D device. Here we are using the default adapter (most
    // systems only have one, unless they have multiple graphics hardware cards
    // installed) and requesting the HAL (which is saying we want the hardware
    // device rather than a software one). Software vertex processing is 
    // specified since we know it will work on all cards. On cards that support 
    // hardware vertex processing, though, we would see a big performance gain 
    // by specifying hardware vertex processing.
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Device state would normally be set here

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// Name: Sfarme()
// Desc: 스킬애니메이션관리
//-----------------------------------------------------------------------------
VOID Sframe() {
		player.IdleState = FALSE;
		if (player.L == TRUE) {
			if (test.visible == TRUE) {
				player.rect = { 0, 68 * 2, 53, 68 * 3 };
			}
			test.pos = { player.pos.x - 50,player.pos.y - 100,0 };
			test2.pos = { player.pos.x - 50,player.pos.y - 100,0 };
			test.visible = TRUE;
			test.frame++;
			if (test.frame == 0) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 3) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
				//test.rect = { 155,0,310,268 };
			}
			if (test.frame == 6) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
				//test.rect = { 310,0,465,268 };
			}
			if (test.frame == 9) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
				//test.rect = { 465,0,620,268 };
			}
			if (test.frame == 12) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 15) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 18) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 21) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 24) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 27) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 30) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 33) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 36) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 39) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 42) {
				test.rect = { test.rx,0,test.rrx,268 };
				test.rx += 155;
				test.rrx += 155;
			}
			if (test.frame == 42) {
				test.rx = 0;
				test.rrx = 155;
				//test.frame = 0;
				test.visible = FALSE;
				test2.visible = TRUE;
			}

			if (test2.visible == TRUE) {
				test2.frame++;
				test.visible = FALSE;
				player.rect = { 55, 68 * 2, 108, 68 * 3 };
				if (test2.frame == 0) {
					test2.rect = { test2.rx,0,test2.rrx,276 };
					test2.rx += 127;
					test2.rrx += 127;
				}
				if (test2.frame == 1) {
					test2.rect = { test2.rx,0,test2.rrx,276 };
					test2.rx += 127;
					test2.rrx += 127;
				}
				if (test2.frame == 2) {
					test2.rect = { test2.rx,0,test2.rrx,276 };
					test2.rx += 127;
					test2.rrx += 127;
				}
				if (test2.frame == 3) {
					test2.rect = { test2.rx,0,test2.rrx,276 };
					test2.rx += 127;
					test2.rrx += 127;
				}
				if (test2.frame == 4) {
					test2.rect = { test2.rx,0,test2.rrx,276 };
					test2.rx += 127;
					test2.rrx += 127;
				}
				if (test2.frame == 5) {
					test2.rect = { test2.rx,0,test2.rrx,276 };
					test2.rx += 127;
					test2.rrx += 127;
				}
				if (test2.frame == 6) {
					test2.rect = { test2.rx,0,test2.rrx,276 };
					test2.rx += 127;
					test2.rrx += 127;
				}
				if (test2.frame == 6) {
					test2.rx = 0;
					test2.rrx = 127;
					test2.frame = 0;
				}
			}
		}
		if (player.R == TRUE) {
			if (test.visible == TRUE) {
				player.rect = { 0, 68 * 2, 53, 68 * 3 };
			}
			test.pos = { player.pos.x + 100,player.pos.y - 100,0 };
			test2.pos = { player.pos.x + 100,player.pos.y - 100,0 };
			test.visible = TRUE;
			test.frame++;
			if (test.frame == 0) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 3) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
				//test.rect = { 155,0,310,268 };
			}
			if (test.frame == 6) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
				//test.rect = { 310,0,465,268 };
			}
			if (test.frame == 9) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
				//test.rect = { 465,0,620,268 };
			}
			if (test.frame == 12) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 15) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 18) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 21) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 24) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 27) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 30) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 33) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 36) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 39) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 42) {
				test.rect = { test.lx,268,test.llx,268 * 2 };
				test.lx -= 155;
				test.llx -= 155;
			}
			if (test.frame == 42) {
				test.lx = 2170;
				test.llx = 2015;
				test.frame = 0;
				test.visible = FALSE;
				test2.visible = TRUE;
			}

			if (test2.visible == TRUE) {
				test2.frame++;
				test.visible = FALSE;
				player.rect = { 55, 68 * 2, 108, 68 * 3 };
				if (test2.frame == 0) {
					test2.rect = { test2.lx,276,test2.llx,276 * 2 };
					test2.lx -= 127;
					test2.llx -= 127;
				}
				if (test2.frame == 1) {
					test2.rect = { test2.lx,276,test2.llx,276 * 2 };
					test2.lx -= 127;
					test2.llx -= 127;
				}
				if (test2.frame == 2) {
					test2.rect = { test2.lx,276,test2.llx,276 * 2 };
					test2.lx -= 127;
					test2.llx -= 127;
				}
				if (test2.frame == 3) {
					test2.rect = { test2.lx,276,test2.llx,276 * 2 };
					test2.lx -= 127;
					test2.llx -= 127;
				}
				if (test2.frame == 4) {
					test2.rect = { test2.lx,276,test2.llx,276 * 2 };
					test2.lx -= 127;
					test2.llx -= 127;
				}
				if (test2.frame == 5) {
					test2.rect = { test2.lx,276,test2.llx,276 * 2 };
					test2.lx -= 127;
					test2.llx -= 127;
				}
				if (test2.frame == 6) {
					test2.rect = { test2.lx,276,test2.llx,276 * 2 };
					test2.lx -= 127;
					test2.llx -= 127;
				}
				if (test2.frame == 6) {
					test2.lx = 762;
					test2.llx = 635;
					test2.frame = 0;
				}
			}
		}
}

//-----------------------------------------------------------------------------
// Name: PIdle()
// Desc: 플레이어 유휴상태일때 애니메이션관리
//-----------------------------------------------------------------------------
VOID PIdle() {
	player.Iframe++;
	if (player.Iframe == 0) {
		player.rect = { player.rx,0,player.rrx,68 };
		player.rx += 56;
		player.rrx += 56;
	}
	if (player.Iframe == 20) {
		player.rect = { player.rx,0,player.rrx,68 };
		player.rx += 56;
		player.rrx += 56;
	}	
	if (player.Iframe == 40) {
		player.rect = { player.rx,0,player.rrx,68 };
		player.rx += 56;
		player.rrx += 56;
	}
	if (player.Iframe == 60) {
		player.rect = { player.rx,0,player.rrx,68 };
		player.rx += 56;
		player.rrx += 56;
	}
	if (player.Iframe == 80) {
		player.rect = { player.rx,0,player.rrx,68 };
		player.rx += 56;
		player.rrx += 56;
	}

	if (player.Iframe == 80) {
		player.Iframe = 0;
		player.rx = 0;
		player.rrx = 56;
	}
}

//-----------------------------------------------------------------------------
// Name: PWalk()
// Desc: 플레이어 걷기상태일때 애니메이션관리
//-----------------------------------------------------------------------------
VOID PWalk() {
	player.IdleState = FALSE;
	player.WalkingState = TRUE;
	player.Wframe++;
	player.pos.x -= 3;
	if (player.Wframe == 0) {
		player.rect = { player.wx,68,player.wwx,68*2 };
		player.wx += 57;
		player.wwx += 57;
	}
	if (player.Wframe == 10) {
		player.rect = { player.wx,68,player.wwx,68*2 };
		player.wx += 57;
		player.wwx += 57;
	}
	if (player.Wframe == 20) {
		player.rect = { player.wx,68,player.wwx,68*2 };
		player.wx += 57;
		player.wwx += 57;
	}
	if (player.Wframe == 30) {
		player.rect = { player.wx,68,player.wwx,68*2 };
		player.wx += 57;
		player.wwx += 57;
	}
	if (player.Wframe == 30) {
		player.Wframe = 0;
		player.wx = 0;
		player.wwx = 57;
	}

}

//-----------------------------------------------------------------------------
// Name: Arrow()
// Desc: 화살 애니메이션관리 68 39
//-----------------------------------------------------------------------------
VOID Arrow() {
	arrow[0].frame++;
	//arrow[0].pos = { player.pos.x - 85,player.pos.y + 20,0 };
	arrow[0].pos.x--;

	if (arrow[0].frame == 0) {
		arrow[0].rect = { arrow[0].rx,0,arrow[0].rrx,39 };
		arrow[0].rx += 68;
		arrow[0].rrx += 68;
	}
	if (arrow[0].frame == 2) {
		arrow[0].rect = { arrow[0].rx,0,arrow[0].rrx,39 };
		arrow[0].rx += 68;
		arrow[0].rrx += 68;
	}
	if (arrow[0].frame == 4) {
		arrow[0].rect = { arrow[0].rx,0,arrow[0].rrx,39 };
		arrow[0].rx += 68;
		arrow[0].rrx += 68;
	}
	if (arrow[0].frame == 6) {
		arrow[0].rect = { arrow[0].rx,0,arrow[0].rrx,39 };
		arrow[0].rx += 68;
		arrow[0].rrx += 68;
	}
	if (arrow[0].frame == 8) {
		arrow[0].rect = { arrow[0].rx,0,arrow[0].rrx,39 };
		arrow[0].rx += 68;
		arrow[0].rrx += 68;
	}
	if (arrow[0].frame == 10) {
		arrow[0].rect = { arrow[0].rx,0,arrow[0].rrx,39 };
		arrow[0].rx += 68;
		arrow[0].rrx += 68;
	}
	if (arrow[0].frame == 12) {
		arrow[0].rect = { arrow[0].rx,0,arrow[0].rrx,39 };
		arrow[0].rx += 68;
		arrow[0].rrx += 68;
	}
	if (arrow[0].frame == 14) {
		arrow[0].rect = { arrow[0].rx,0,arrow[0].rrx,39 };
		arrow[0].rx += 68;
		arrow[0].rrx += 68;
	}
	if (arrow[0].frame == 16) {
		arrow[0].rect = { arrow[0].rx,0,arrow[0].rrx,39 };
		arrow[0].rx += 68;
		arrow[0].rrx += 68;
	}
	if (arrow[0].frame == 18) {
		arrow[0].rect = { arrow[0].rx,0,arrow[0].rrx,39 };
		arrow[0].rx += 68;
		arrow[0].rrx += 68;
	}
	if (arrow[0].frame == 18) {
		arrow[0].frame = 0;
		arrow[0].rx = 0;
		arrow[0].rrx = 68;
	}
}

/*VOID MIdle() {
	monster.Iframe++;
	if (monster.Iframe == 0) {
		monster.rect = { monster.rx,0,monster.rrx,52 };
		monster.rx += 56;
		monster.rrx += 56;
	}
	if (monster.Iframe == 20) {
		monster.rect = { monster.rx,0,monster.rrx,52 };
		monster.rx += 56;
		monster.rrx += 56;
	}
	if (monster.Iframe == 40) {
		monster.rect = { monster.rx,0,monster.rrx,52 };
		monster.rx += 56;
		monster.rrx += 56;
	}
	if (monster.Iframe == 40) {
		monster.Iframe = 0;
		monster.rx = 0;
		monster.rrx = 56;
	}
}*/

VOID MWalk() {
	for (int i = 0; i < 10; i++) {
		if (wave == 1) {
			if (monster[i].visible == TRUE) {
				monster[i].pos.x++;
				monster[i].frame++;
				if (monster[i].frame == 0) {
					monster[i].rect = { 0,0,63,64 };
					monster[i].wx += 56;
					monster[i].wwx += 56;
				}
				if (monster[i].frame == 20) {
					monster[i].rect = { 63,0,125,64 };
					monster[i].wx += 56;
					monster[i].wwx += 56;
				}
				if (monster[i].frame == 40) {
					monster[i].rect = { 125,0,189,64 };
					monster[i].wx += 56;
					monster[i].wwx += 56;
				}
				/*if (monster[i].frame == 60) {
				monster[i].rect = { monster[i].wx,0,monster[i].wwx,64 };
				monster[i].wx += 56;
				monster[i].wwx += 56;
				}*/
				if (monster[i].frame == 40) {
					monster[i].frame = 0;
					monster[i].wx = 0;
					monster[i].wwx = 56;
				}
			}
		}
		if (wave == 2) {
			if (monster[i].visible == TRUE) {
				monster[i].pos.x++;
				monster[i].frame++;
				if (monster[i].frame == 0) {
					monster[i].rect = { monster[i].wx,64,monster[i].wwx,116 };
					monster[i].wx += 56;
					monster[i].wwx += 56;
				}
				if (monster[i].frame == 20) {
					monster[i].rect = { monster[i].wx,64,monster[i].wwx,116 };
					monster[i].wx += 56;
					monster[i].wwx += 56;
				}
				if (monster[i].frame == 40) {
					monster[i].rect = { monster[i].wx,64,monster[i].wwx,116 };
					monster[i].wx += 56;
					monster[i].wwx += 56;
				}
				if (monster[i].frame == 60) {
				monster[i].rect = { monster[i].wx,64,monster[i].wwx,116 };
				monster[i].wx += 56;
				monster[i].wwx += 56;
				}
				if (monster[i].frame == 60) {
					monster[i].frame = 0;
					monster[i].wx = 0;
					monster[i].wwx = 56;
				}
			}
		}
		if (wave == 3) {
			if (monster[i].visible == TRUE) {
				monster[i].pos.x++;
				monster[i].frame++;
				if (monster[i].frame == 0) {
					monster[i].rect = { monster[i].wx,116,monster[i].wwx,168 };
					monster[i].wx += 56;
					monster[i].wwx += 56;
				}
				if (monster[i].frame == 20) {
					monster[i].rect = { monster[i].wx,116,monster[i].wwx,168 };
					monster[i].wx += 56;
					monster[i].wwx += 56;
				}
				if (monster[i].frame == 40) {
					monster[i].rect = { monster[i].wx,116,monster[i].wwx,168 };
					monster[i].wx += 56;
					monster[i].wwx += 56;
				}
				if (monster[i].frame == 60) {
					monster[i].rect = { monster[i].wx,116,monster[i].wwx,168 };
					monster[i].wx += 56;
					monster[i].wwx += 56;
				}
				if (monster[i].frame == 60) {
					monster[i].frame = 0;
					monster[i].wx = 0;
					monster[i].wwx = 56;
				}
			}
		}

	}
}
//BOOL CheckAABB()
//{
//	for (int i = 0; i < 100; i++) {
//		if (arrow[i].pos.x + arrow[i].rect.left < monster.pos.x + monster.rect.right && arrow[i].pos.x + arrow[i].rect.right > monster.pos.x + monster.rect.left &&
//			arrow[i].pos.y + arrow[i].rect.top < monster.pos.y + monster.rect.bottom && arrow[i].pos.y + arrow[i].rect.bottom > monster.pos.y + monster.rect.top)
//		{
//			return true;
//		}
//		else {
//			return false;
//		}
//	}
//
//}

//BOOL aabb() {
//	for (int i = 0; i < 100; i++) {
//		if (arrow[i].rect.left < monster.rect.left + monster.rect.right &&
//			arrow[i].rect.left + arrow[i].rect.right > monster.rect.left &&
//			arrow[i].rect.top < monster.rect.top + monster.rect.bottom &&
//			arrow[i].rect.bottom + arrow[i].rect.top > monster.rect.top) {
//			return true;
//		}
//		else {
//			return false;
//		}
//
//
//		//monster.rect = { 0,0,56,52 };
//	}
//}

BOOL Collision(D3DXVECTOR3 a, float r1, D3DXVECTOR3 b, float r2) {
	float dist = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y) *(a.y - b.y));
	if (dist <= r1 + r2) {
		return true;
	}
	else {
		return false;
	}
}

//VOID MDie() {
//	monster.frame++;
//	if (monster.frame == 0) {
//		monster.rect = { monster.dx,0,monster.ddx,52 * 4 };
//		monster.dx += 57;
//		monster.ddx += 57;
//	}
//	if (monster.frame == 20) {
//		monster.rect = { monster.dx,0,monster.ddx,52 * 4 };
//		monster.dx += 57;
//		monster.ddx += 57;
//	}
//	if (monster.frame == 40) {
//		monster.rect = { monster.dx,0,monster.ddx,52 };
//		monster.dx += 57;
//		monster.ddx += 57;
//	}
//	if (monster.frame == 60) {
//		monster.rect = { monster.dx,0,monster.ddx,52 };
//		monster.dx += 57;
//		monster.ddx += 57;
//	}
//	if (monster.frame == 60) {
//		monster.frame = 0;
//		monster.dx = 0;
//		monster.ddx = 57;
//	}
//}

VOID Init() {
	D3DXCreateSprite(g_pd3dDevice, &g_pSprite);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"open.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &tur1.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"open2.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &tur2.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"wave1.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &wave1.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"wave2.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &wave2.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"wave3.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &wave3.texture);


	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"testcoll.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &testcoll.texture);
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"schar.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &player.texture);
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"start1.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &test.texture);
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"ing1.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &test2.texture);
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"arrow.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &arrow[0].texture);
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"mon.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &monster[0].texture);
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"main800.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &init.texture);
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"map2.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &map.texture);
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"hp.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &hp.texture);
	
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"clear.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &clear.texture);
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"clearmsg.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &clearmsg.texture);
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"fail.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &fail.texture);
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"failmsg.png",
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &failmsg.texture);






	init.rect = { 0,0,800,600 };

	tur1.rect = { 0, 0, 800, 600 };
	tur2.rect = { 0, 0, 800, 600 };
	tur2.visible = FALSE;
	wave1.rect = { 0, 0, 800, 600 };
	wave1.visible = FALSE;
	wave2.rect = { 0, 0, 800, 600 };
	wave2.visible = FALSE;
	wave3.rect = { 0, 0, 800, 600 };
	wave3.visible = FALSE;

	map.rect = { 0, 0, 800, 600 };

	hp.rect = { 0,0,350,50 };
	hp.pos = { 75,50, 0 };

	clear.rect = { 0,0,800,600 };
	fail.rect = { 0,0,800,600 };
	failmsg.rect = { 0,0,465,150 };
	clearmsg.rect = { 0,0,465,150 };
	clearmsg.pos = { 175,100,0 };
	failmsg.pos = { 175,100,0 };
	clear.visible = FALSE;
	fail.visible = FALSE;
	clearmsg.visible = FALSE;
	failmsg.visible = FALSE;

	//오른쪽값이상하면 x값 3~4빼기

	//test.rect = { 0,0,155,268 };
	
	testcoll.rect = { 0,0,200,200 };
	testcoll.center = { 0,0,0 };
	testcoll.pos = { 300,300,0 };
	testcoll.visible = FALSE;

	player.center = { 0,0,0 };
	player.pos = { 700,510,0 };
	player.Iframe = 0;
	player.visible = TRUE;
	player.IdleState = TRUE;
	player.WalkingState = FALSE;
	player.rx = 0;
	player.rrx = 56;
	player.wx = 0;
	player.wwx = 57;
	player.L = TRUE;
	player.R = FALSE;

	
	test.center = { 0,0,0 };
	test.pos = { player.pos.x - 50,player.pos.y - 100,0 };
	test.frame = 0;
	test.visible = TRUE;
	test.rx = 0;
	test.rrx = 155;
	test.lx = 2170;
	test.llx = 2015;
	
	test2.center = { 0,0,0 };
	test2.pos = { player.pos.x - 50,player.pos.y - 100,0 };
	test2.frame = 0;
	test2.visible = FALSE;
	test2.rx = 0;
	test2.rrx = 127;
	test2.lx = 762;
	test2.llx = 635;

	arrow[0].center = { 0,0,0 };
	arrow[0].pos = { player.pos.x - 85,player.pos.y + 20,0 };
	arrow[0].frame = 0;
	arrow[0].visible = FALSE;
	arrow[0].rx = 0;
	arrow[0].rrx = 68;

	for (int i = 1; i < 100; i++) {
		arrow[i] = arrow[0];
	}

	//monster[0].rect = { 0,0,56,52 };
	monster[0].pos = { 100,526,0 };
	monster[0].rx = 0;
	monster[0].rrx = 56;
	monster[0].dx = 57;
	monster[0].ddx = 0;
	monster[0].wx = 0;
	monster[0].wwx = 56;
	monster[0].visible = FALSE;
	monster[0].HP = 100;

	for (int i = 1; i < 10; i++) {
		monster[i] = monster[0];
	}

	SpawnManager = monster[0];

	//player.rect = { 0,0,57,68 };

}

VOID Update() {

	clock_t CurTime = clock();//시간

	//esc
	if (GetKeyState(VK_ESCAPE) & 0x80000000) {
		Cleanup();
		PostQuitMessage(0);
		ExitProcess(0);
	}


	switch (g_GameState) {
	case INIT:

		if (CurTime - g_OldTime > 5000) {
			//g_OldTime = CurTime;
			g_GameState = READY;
		}

		//if (GetKeyState(VK_DELETE) & 0x80000000) {
		//	g_GameState = READY;
		//}
		
		break;
	case READY:

		if (CurTime - g_OldTime > 10000) {
			//g_OldTime = CurTime;
			tur2.visible = TRUE;
			//g_GameState = READY;
		}
		if (GetKeyState(VK_SPACE) & 0x80000000) {
			g_GameState = RUNNING;
		}
		break;
	case RUNNING:

		if (wavectrl == TRUE) {
			if (wave == 1) {
				wave1.visible = TRUE;
				wavedelay++;
				if (wavedelay == 100) {
					wavedelay = 0;
					wave1.visible = FALSE;
					wavectrl = FALSE;
				}
			}
			if (wave == 2) {
				wave2.visible = TRUE;
				wavedelay++;
				if (wavedelay == 100) {
					wavedelay = 0;
					wave2.visible = FALSE;
					wavectrl = FALSE;
				}
			}
			if (wave == 3) {
				wave3.visible = TRUE;
				wavedelay++;
				if (wavedelay == 100) {
					wavedelay = 0;
					wave3.visible = FALSE;
					wavectrl = FALSE;
				}
			}
		}




		//총알은 항상 옆으로감
		for (int i = 0; i < 100; i++) {
			if (arrow[i].visible == TRUE) {
				for (int a = 0; a < 100; a++) {
					arrow[a].pos.x -= 10;
					if (arrow[a].pos.x < -60) {
						arrow[a].visible = FALSE;

					}
					else {
						arrow[a].visible = TRUE;
					}

				}
			}
		}


		


		if (GetKeyState(VK_LSHIFT) & 0x80000000) {
			//Arrow();

			//for (int i = 0; i < 100; i++) {
			//	if (arrow[i].visible == FALSE) {

			//		arrow[i].visible = TRUE;
			//		arrow[i].pos = { player.pos.x - 85,player.pos.y + 20,0 };
			//		break;
			//	}
			//}
			//arrow.pos = { 300,320,0 };
		}


		if (player.IdleState == TRUE) {
			PIdle();
		}

		if (GetKeyState(VK_RIGHT) & 0x80000000) {
			player.L = FALSE;
			player.R = TRUE;
			player.pos.x += 3;
		}


		if (GetKeyState(VK_LEFT) & 0x80000000) {
			player.L = TRUE;
			player.R = FALSE;
			PWalk();
		}
		else {
			player.IdleState = TRUE;
		}

		if (player.DieState == TRUE) {
			player.IdleState = FALSE;
			player.rect = { 57 * 4,68,57 * 5,68 * 2 };
			//player.rect = { 232,85,277,135 };
		}
		else {
			player.IdleState = TRUE;
		}


		if (GetKeyState(VK_LCONTROL) & 0x80000000) {
			player.pos = { 700,300,0 };
		}

		if (GetKeyState(VK_SPACE) & 0x80000000) {
			Sframe();
			if (test2.visible == TRUE) {
				Arrow();

				for (int i = 0; i < 100; i++) {
					if (arrow[i].visible == FALSE) {

						arrow[i].visible = TRUE;
						arrow[i].pos = { player.pos.x - 85,player.pos.y + 20,0 };
						break;
					}
				}
			}

		}
		else {
			test.visible = FALSE;
			test2.visible = FALSE;
			test.frame = 0;
			test2.frame = 0;
			test.rx = 0;
			test.rrx = 155;
			test2.rx = 0;
			test2.rrx = 127;
		}

		/*if (CurTime - g_OldTime > 1000) {
			g_OldTime = CurTime;
			for (int i = 0; i < 10; i++) {
				monster[i].visible = TRUE;
			}
		}*/

		delay++;

		//if (delay == 100) {
		//	monster[0].visible = TRUE;
		//}
		//if (delay == 200) {
		//	monster[1].visible = TRUE;
		//}


			if (delay == 100) {
				monster[s].visible = TRUE;
				s++;
				delay = 0;
				if (s == 9) {
					s = 0;
				}
			}
			MWalk();

			for (int i = 0; i < 10; i++) {
				if (monster[i].pos.x > 800) {
					monster[i] = SpawnManager;
					if (hp.rect.right >= 150) {
						hp.rect.right -= 50;
					}

					deathcnt++;
				}
			}

			for (int i = 0; i < 10; i++) {
				if (monster[i].HP == 0) {
					monster[i] = SpawnManager;
				}
			}

			for (int i = 0; i < 100; i++) {
				for (int a = 0; a < 10; a++) {
					if (Collision(arrow[i].pos, 20, monster[s].pos, 50)) {
						arrow[i].visible = FALSE;
						monster[a].HP--;
						if (monster[a].HP == 0) {
							monster[a].visible = FALSE;
							killcnt++;
							monster[a] = SpawnManager;
						}

					}
				}
			}
			if (killcnt == 10) {
				killcnt = 0;
				deathcnt = 0;
				wave++;
				wavectrl = TRUE;
			}
			if (wave == 4) {

					g_GameState = SUCCESS;
			}
			if (deathcnt == 5) {
				player.DieState = TRUE;
				//player.IdleState = FALSE;
				killcnt = 0;
				deathcnt = 0;

				//Sleep(3000);

				failmsg.visible = TRUE;


				g_GameState = FAILED;
		


				//player.rect = { 232,85,278,135 };
				//g_GameState = INIT;

				//player.rect = { 57 * 3,69,57 * 4,69 * 2 };
				//player.pos.y += 4;
			}
		break;
	case STOP:
		break;
	case SUCCESS:
		break;
	case FAILED:
		break;
	case RESULT:
		break;
	}

	
	
	//test.rect = { 155, 0,310,268 };

	
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
    if( NULL == g_pd3dDevice )
        return;

    // Clear the backbuffer to a blue color
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 255 ), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
        // Rendering of scene objects can happen here
		g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);


		switch (g_GameState) {
		case INIT:
			g_pSprite->Draw(init.texture, &init.rect, &init.center, &init.pos, 0xffffffff);
			break;
		case READY:
			g_pSprite->Draw(tur1.texture, &tur1.rect, &tur1.center, &tur1.pos, 0xffffffff);
			if (tur2.visible == TRUE) {
				g_pSprite->Draw(tur2.texture, &tur2.rect, &tur2.center, &tur2.pos, 0xffffffff);
			}
			break;
		case RUNNING:
			g_pSprite->Draw(map.texture, &map.rect, &map.center, &map.pos, 0xffffffff);
			g_pSprite->Draw(hp.texture, &hp.rect, &hp.center, &hp.pos, 0xffffffff);
			if (testcoll.visible == TRUE) {
				g_pSprite->Draw(testcoll.texture, &testcoll.rect, &testcoll.center, &testcoll.pos, 0xffffffff);
			}

			g_pSprite->Draw(player.texture, &player.rect, &player.center, &player.pos, 0xffffffff);

			for (int i = 0; i < 100; i++) {
				if(arrow[i].visible == TRUE)
				g_pSprite->Draw(arrow[i].texture, &arrow[i].rect, &arrow[i].center, &arrow[i].pos, 0xffffffff);
			}
			for (int i = 0; i < 10; i++) {
				if (monster[i].visible == TRUE) {
					g_pSprite->Draw(monster[i].texture, &monster[i].rect, &monster[i].center, &monster[i].pos, 0xffffffff);
				}
			}



			if (test.visible == TRUE) {
				g_pSprite->Draw(test.texture, &test.rect, &test.center, &test.pos, 0xffffffff);
			}

			if (test2.visible == TRUE) {
				g_pSprite->Draw(test2.texture, &test2.rect, &test2.center, &test2.pos, 0xffffffff);
			}
			if (wave1.visible == TRUE) {
				g_pSprite->Draw(wave1.texture, &wave1.rect, &wave1.center, &wave1.pos, 0xffffffff);
			}
			if (wave2.visible == TRUE) {
				g_pSprite->Draw(wave2.texture, &wave2.rect, &wave2.center, &wave2.pos, 0xffffffff);
			}
			if (wave3.visible == TRUE) {
				g_pSprite->Draw(wave3.texture, &wave3.rect, &wave3.center, &wave3.pos, 0xffffffff);
			}
			if (clearmsg.visible == TRUE) {
				g_pSprite->Draw(clearmsg.texture, &clearmsg.rect, &clearmsg.center, &clearmsg.pos, 0xffffffff);
			}
			if (failmsg.visible == TRUE) {
				g_pSprite->Draw(failmsg.texture, &failmsg.rect, &failmsg.center, &failmsg.pos, 0xffffffff);
			}



			break;

		case STOP:

			break;
		case SUCCESS:
			g_pSprite->Draw(clear.texture, &clear.rect, &clear.center, &clear.pos, 0xffffffff);
			break;
		case FAILED:
			g_pSprite->Draw(fail.texture, &fail.rect, &fail.center, &fail.pos, 0xffffffff);
			break;
		case RESULT:

			break;
		}



		
		
		g_pSprite->End();
        // End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            Cleanup();
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    UNREFERENCED_PARAMETER( hInst );

    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        L"D3D Tutorial", NULL
    };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"MapleStory",
                              WS_EX_TOPMOST | WS_POPUP, 0, 0, 800, 600,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
		Init();
        // Show the window
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );

        // Enter the message loop
        MSG msg;
		ZeroMemory(&msg, sizeof(msg));
        while( msg.message != WM_QUIT )
        {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				Update();
				Render();
			}

        }
    }

    UnregisterClass( L"D3D Tutorial", wc.hInstance );
    return 0;
}



