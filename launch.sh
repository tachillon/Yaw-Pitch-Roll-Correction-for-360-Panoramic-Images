WORKDIR_PATH=/Users/achille/Desktop/WarpPanorama
docker rm $(docker ps -a -q)
docker rmi $(docker images --filter "dangling=true" -q --no-trunc)
if docker build -t stabilize:1 . ; then 
    echo "launching program..."
    mkdir $WORKDIR_PATH/res
    docker run --rm -it -v $WORKDIR_PATH:/tmp stabilize:1 stabilizePano
    echo "...End"
fi