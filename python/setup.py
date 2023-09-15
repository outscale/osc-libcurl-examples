# -*- coding:utf-8 -*-
from setuptools import find_packages, setup

setup(
    name='osc-libcurl-examples-python',
    version='1.0.0',
    packages=find_packages(),
    author='Outscale SAS',
    author_email='opensource@outscale.com',
    description='Outscale',
    url='http://www.outscale.com/',
    entry_points={'console_scripts': ['example01 = example01:main']},
    install_requires=[
        'pycurl @ git+https://github.com/pycurl/pycurl@master'
    ],
)
