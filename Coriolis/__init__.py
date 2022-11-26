#This is needed for poetry to recognise the top level module
import os

__all__ = ["crlcore"]
coriolis_package_dir = os.path.abspath(os.path.dirname(__file__))

os.environ["CORIOLIS_TOP"] = coriolis_package_dir
