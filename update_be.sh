#!/bin/bash

CONTAINER=fc-fitcrack_server-1
ROOT_INSIDE=/var/www/html
PATH_TO_API=$1

# check if cwd is the root of the project
if [ ! -f "Dockerfile" ]; then
  echo "You must run this script from the root of the project"
  exit 1
fi

# Set default value for PATH_TO_API if not provided
if [ -z "$PATH_TO_API" ]; then
  PATH_TO_API="fitcrackAPI/src/src"
  echo "No path provided, using default path: $PATH_TO_API"
fi

# remove the old directory inside the container
docker exec -t $CONTAINER rm -rf $ROOT_INSIDE/$PATH_TO_API
# copy the new directory inside the container
docker cp webadmin/$PATH_TO_API $CONTAINER:$ROOT_INSIDE/$PATH_TO_API
# restart the server inside the container
docker exec -t $CONTAINER service apache2 restart