echo "UPDATE server sources in the docker"
CONTAINER_NAME=fitcrack_fitcrack_server_1
echo "Assuming container $CONTAINER_NAME is running"

#patch -p0 < installer/fitcrack_changes_in_boinc.patch

# Copy server files to BOINC server Root
echo "copying Generator and Assimilator sources to the BOINC directory"

for file in server/src/headers/*.h; do
    echo "Copying $file"
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/server/src/headers/
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/boinc/sched/
done

for file in server/src/headers/AttackModes/*.h; do
    echo "Copying $file"
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/server/src/headers/AttackModes/
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/boinc/sched/
done

for file in server/src/headers/Generators/*.h; do
    echo "Copying $file"
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/server/src/headers/Generators/
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/boinc/sched/
done

for file in server/src/headers/Database/*.h; do
    echo "Copying $file"
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/server/src/headers/Database/
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/boinc/sched/
done

for file in server/src/headers/Utils/*.h; do
    echo "Copying $file"
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/server/src/headers/Utils/
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/boinc/sched/
done

for file in server/src/source/*.cpp; do
    echo "Copying $file"
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/server/src/source/
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/boinc/sched/
done

for file in server/src/source/AttackModes/*.cpp; do
    echo "Copying $file"
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/server/src/source/AttackModes/
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/boinc/sched/
done

for file in server/src/source/Generators/*.cpp; do
    echo "Copying $file"
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/server/src/source/Generators/
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/boinc/sched/
done    

for file in server/src/source/Database/*.cpp; do
    echo "Copying $file"
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/server/src/source/Database/
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/boinc/sched/
done

for file in server/src/source/Utils/*.cpp; do
    echo "Copying $file"
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/server/src/source/Utils/
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/boinc/sched/
done

for file in server/src/include/*; do
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/server/src/include/
    docker cp "$file" $CONTAINER_NAME:/srv/fitcrack/boinc/sched/
done

echo "Update of sources done."
echo "======================="
echo "You can enter the container and type:"
echo "cd /srv/fitcrack/boinc/ && make"
echo "OR, if you want just Generator, Assimilator, and Validator, do:"
echo "cd /srv/fitcrack/boinc/sched && make"
echo "Once compiled, copy the binaries to /home/boincadm/projects/fitcrack/bin/"