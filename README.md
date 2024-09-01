# cpp_tutorial

## Install vscode
vscode is a populer and comprehensive integrated development environment (IDE) with millions of extensions you can add to accomodate various tasks. The minimum function of it is a text editor, but with extensions, it can be used as interactive code development environment with debugger functions. Download from https://code.visualstudio.com/

## Install g++ on windows
For windows 8 +, please follow the vscode tutorial https://code.visualstudio.com/docs/cpp/config-mingw to install the g++ compiler on windows.  

## install visual studio 2022 on windows
The visual studio 2022 provides many c++ development tools that can be helpful. Follow the link: https://visualstudio.microsoft.com/zh-hans/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false


## install git (highly recommended)
Follow https://git-scm.com/downloads , install git. Git is a version control tool that is widely used. In the setup, do not use the credential helper. Then you need to configure your git credentials: 
    git config --global user.name "your_name"
    git config --global user.email "your_email"

For windows, do the following also:
    git config --global core.autocrlf true

One can view the official documents and understand the basic syntax of Git. For now, we generaly use the following two lines:
    git add .
    git commit -m "You commit message"
Note that some convention on the commit message is to start with a capitalized verb, e.g. git commit -m "Add linear algebraic functions".

## config remote repository (recommended)
If you want to deploy code in different machines or simply share the code and have others contribute to it, you can use an online code repository. Some examples would be github and gitlab etc. For github, go to https://github.com/ , and follow the instructions to register an account with an email and a password. The way to push/pull your code to/from the repo is via ssh. To use ssh, you need to setup the ssh key and use the public key for communication with the repo. Use the following code to generate the ssh key 
    ssh-keygen -t rsa -b 2048
This will prompt you to enter a passphrase for your key, you can choose to leave it empty by hitting enter. After the key is generated, there will be two files under folder C:\Users\your_user_name\.ssh (for windows). One is the key, and the other is the corresponding public key with .pub extension. You can use notepad to open the public key and copy and paste it in the remote repo. After this, you should be able to create a repo on your remote and use the git to clone that repo to your local machine with the following 
    git clone git@your_remote_repo_name
After you have cloned your remote repo to your local machine, you can make modifications to it and use Git as the version control and now you can push the changes to the remote with 
    git push
