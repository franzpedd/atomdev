import os;
import sys;
import platform;
import subprocess;
import shutil;

import requests;
from tqdm import tqdm;

class Utils :

    @classmethod
    def Download(cls, url, path) :

        if(os.path.isfile(path) is True) : return;

        downloadrequest = requests.get(url, stream = True);
        downloadpercentage = int(downloadrequest.headers.get("content-lenght", 0));

        with open(path, "wb") as file, tqdm(
                desc = path, total = downloadpercentage, unit = "iB", unit_scale = True, unit_divisor = 1024,
            ) as bar :
                for data in downloadrequest.iter_content(chunk_size = 1024) :
                    size = file.write(data);
                    bar.update(size);
    
    @classmethod
    def Unzip(cls, file, path) :

        if(os.path.isfile(file) is None) : return;

        shutil.unpack_archive(file, path);

class Premake5 :

    version = "5.0.0-beta1";
    path = "Thirdparty/premake5";

    @classmethod
    def Download(cls) :

        if(os.path.isdir(f"{cls.path}") is False) : os.makedirs(f"{cls.path}");

        url = "";
        file = "";

        if(platform.system() == "Windows") :
            url = f"https://github.com/premake/premake-core/releases/download/v{cls.version}/premake-{cls.version}-windows.zip";
            file = f"{cls.path}/premake-{cls.version}-windows.zip";

        if(platform.system() == "Linux") :
            url = f"https://github.com/premake/premake-core/releases/download/v{cls.version}/premake-{cls.version}-linux.tar.gz";
            file = f"{cls.path}/premake-{cls.version}-linux.tar.gz";

        if(platform.system() == "Darwin") :
            url = f"https://github.com/premake/premake-core/releases/download/v{cls.version}/premake-{cls.version}-macosx.tar.gz";
            file = f"{cls.path}/premake-{cls.version}-macosx.tar.gz";

        Utils.Download(url, file);
        Utils.Unzip(file, cls.path);

    @classmethod
    def Generate(cls) :
        if(len(sys.argv) != 2) :
            subprocess.call(f"{cls.path}/premake5 gmake2");
            return;

        param_1= sys.argv[1];
        subprocess.call(f"{cls.path}/premake5 {param_1}");

Premake5.Download();
Premake5.Generate();