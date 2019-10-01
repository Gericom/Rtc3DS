#include <nds.h>
#include <stdio.h>


int main()
{
	consoleDemoInit();  //setup the sub screen for printing

	//set mode 0, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);

	// initialize gl
	glInit(); 

	// enable antialiasing
	//glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	//this should work the same as the normal gl call


	//any floating point gl call is being converted to fixed prior to being implemented

	while(true)
	{
		glViewport(0,0,255,191);
		u32 cpad = *(vu32*)0x0CFFFE70;
		
		vs16* gyro = (vs16*)0x0CFFFE78;

		//u16 cpadX = cpad & 0xFFF;
		//u16 cpadY = (cpad >> 12) & 0xFFF;
		
		u16 cpadX = (~gyro[0] & 0xFFF) ^ 0x800;
		u16 cpadY = (gyro[1] & 0xFFF) ^ 0x800;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-128, 128, -96, 96, 0.1, 100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//not a real gl function and will likely change
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

		glPushMatrix();
		glScalef32(128 * 4096, 128 * 4096, 4096);

		*(vu32*)0x040004A8 = 0;
		//draw the obj
		glBegin(GL_QUAD);

			glColor3b(255,255,255);
			glVertex3v16(-2048,-2048,inttov16(-3));

			glVertex3v16(2048, -2048, inttov16(-3));

			glVertex3v16(2048, 2048, inttov16(-3));

			glVertex3v16(-2048, 2048, inttov16(-3));

		glEnd();

		glBegin(GL_TRIANGLE);
		glColor3b(255,0,0);
		glVertex3v16(0,0,inttov16(-2));
		glVertex3v16(cpadX - 2048,cpadY - 2048,inttov16(-2));
		glVertex3v16(0,0,inttov16(-2));

		glEnd();

		glPopMatrix(1);
		glPushMatrix();
		glTranslatef32(0, -80 * 4096, 4096);
		glScalef32(128 * 4096, 16 * 4096, 4096);

		glBegin(GL_QUAD);

			glColor3b(255,0,0);			

			glVertex3v16(-2048, 2048, inttov16(-2));

			glVertex3v16(-2048,-2048,inttov16(-2));	

			glVertex3v16(cpadX - 2048, -2048, inttov16(-2));	
			
			glVertex3v16(cpadX - 2048, 2048, inttov16(-2));		

		glEnd();

		glPopMatrix(1);
		glPushMatrix();
		glTranslatef32(100 * 4096, 0, 4096);
		glScalef32(16 * 4096, 128 * 4096, 4096);

		glBegin(GL_QUAD);

			glColor3b(255,0,0);			

			glVertex3v16(-2048, cpadY - 2048, inttov16(-2));

			glVertex3v16(-2048,-2048,inttov16(-2));	

			glVertex3v16(2048, -2048, inttov16(-2));	
			
			glVertex3v16(2048, cpadY - 2048, inttov16(-2));		

		glEnd();


		glPopMatrix(1);

		glFlush(0);
		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_B)
			printf("Button B\n");
		
		printf("Data: %x\n", *(vu8*)0x0CFFFE74);
	}

	return 0;
}
