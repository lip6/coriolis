from setuptools import setup

setup(
    name='coriolis',
    version='2.5.5',    
    description='Coriolis Test python package',
    url='https://github.com/lip6/coriolis',
    author='Mazher Iqbal',
    author_email='mazher.iqbal@lip6.fr',
    license='BSD 2-clause',
    packages=['coriolis'],
    install_requires=['mpi4py>=2.0',
                      'numpy',                     
                      ],

    classifiers=[
        'Development Status :: 1 - Planning',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: BSD License',  
        'Operating System :: POSIX :: Linux',        
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
    ],
)
