# Step-by-step preparation of FW development environment

First create virtual environment in root directory. 
Install base requirements (requirements_base.txt).

```bash
python3.9 -m venv .venv
source .venv/bin/activate
pip install -r requirements_base.txt
cd device
west init -l .
cd ..
west update
pip install -r requirements.txt
```

Now you can try to build FW.

```bash
cd device
west build -b nrf52840dk/nrf52840
west flash
```