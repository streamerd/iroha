# How to build `iroha` using docker

Clone `iroha.git` on your directory.

```
git clone https://github.com/hyperledger/iroha.git
cd iroha
```

Build depends on the environment variable `IROHA_HOME` so you need to set it:

`export IROHA_HOME=$(pwd)`

Build on docker container for develop environment `hyperledger/iroha-dev` so you need to build it:

```
cd docker/dev
docker build -t hyperledger/iroha-dev .
```

Run build script and wait for completion. 

```bash
${IROHA_HOME}/docker/build_in_docker.sh
``` 

Docker image with tag `hyperledger/iroha-docker:latest` will be built.

# How to run

```
docker run -d --name=iroha -p 50051:50051 hyperledger/iroha-docker:latest
```


# Troubleshooting

To successfully build docker image, you should be able to build it locally: [follow this guilde](../docs/how_to_build.rst) in case of troubles.


# Development environments
Dev environments aimed to deploy different configurations for development purpose. 

## Ametsuchi
Start different containers:
    * node - container for deploy irohad binary and build project 
    * postgre - container for WSW
    * redis - container for indexes

For run use:
```sh
sh run-ametsuchi-dev.sh
```

## Consnesus
Currently starts only one container - node, but in future it should deploy also container with ordering service.
 
 For run use:
```sh
sh run-consensus-dev.sh
```