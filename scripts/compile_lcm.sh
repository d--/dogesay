if [[ $(docker images -q dogesay:latest) == "" ]]; then 
  docker build -t dogesay .    
fi

echo $PWD

docker run --rm -v $PWD:/home/dogesay -w=/home/dogesay dogesay /bin/bash -c "cd src && lcm-gen -c say.lcm"
