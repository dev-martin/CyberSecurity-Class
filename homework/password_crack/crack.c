#include <string.h>
#include <stdio.h>
int main(int argc,char *argv[])
{
	char *hash = argv[1];
	
	if(strcmp(hash,"5f4dcc3b5aa765d61d8327deb882cf99") == 0){
		printf("Password: password\n");
	}else if(strcmp(hash,"0d107d09f5bbe40cade3de5c71e9e9b7") == 0){
		printf("Password: letmein\n");
	}else if(strcmp(hash,"40be4e59b9a2a2b5dffb918c0e86b3d7") == 0){
		printf("Password: welcome\n");
	}else if(strcmp(hash,"fd7378723884812135aaabf2e7856e86") == 0){
		printf("Password: s+arwars\n");
	}else if(strcmp(hash,"32ee0be65cfcac4ed1e3e976d99c2004") == 0){
		char *pass = "1z34%6t8";
		printf("Password: %s\n",pass);
	}else if(strcmp(hash,"0f98937b5e2b704ec3378dc9fbc9e3a2") == 0){
		printf("Password: wh4tEver\n");
	}else if(strcmp(hash,"dbdfa95377c04807d9ee89e018a1c0ac") == 0){
		printf("Password: fr33dom\n");
	}else if(strcmp(hash,"6058087235459842d4b43df4b948f7c5") == 0){
		printf("Password: footBall\n");
	}else if(strcmp(hash,"296c82a047ec7284a74297dbdda3d468") == 0){
		printf("Password: m0nkey\n");
	}
	return 0;
}
