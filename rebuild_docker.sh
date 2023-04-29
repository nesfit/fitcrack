./remove_docker_installation.sh &&
yes | docker system prune -a --volumes &&
docker-compose build &&
docker-compose up