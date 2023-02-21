# Simple Web Server

I made this web server just for fun and to learn how HTTP works.
To use this server web you need theses tools  :
-  Linux Distribution
-  Docker
	
1. Launch container

		docker run -p 8080:8080  -v "$(pwd)/your_http_project:/app/www-data/" fouegereregent39/simple-web-server
		
2. To connect to web server

Use this link in your browser : localhost:8080/