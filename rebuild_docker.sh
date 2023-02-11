./remove_docker_installation.sh &&
yes | docker system prune -a --volumes &&
docker-compose -f docker-compose-custom-build.yml build &&
docker-compose -f docker-compose-custom-build.yml up