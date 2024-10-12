
import requests
from dataclasses import dataclass, asdict

DEVICE_SERIAL_NO = "0xf4ce3620277fadba"
BACKEND_URL = "http://localhost:8000/devices/"

@dataclass
class Device:
    serial_no: str
    user_name: str = ""
    user_location: str = ""

    def __str__(self):
        output = f"""
Device:
    serial_no:  {self.serial_no}
    user_name:  {self.user_name}
    user_location:  {self.user_location}
"""
        return output

def add_device(device: Device):
    response = requests.post(
        url=BACKEND_URL,
        json=asdict(device)
    )
    print(f"Response: {response.status_code}")

def update_device(device: Device):
    url = f"{BACKEND_URL}{device.serial_no}/"
    response = requests.put(
        url=url,
        json=asdict(device)
    )
    print(f"Response: {response.status_code}")

def get_device(serial_no: str) -> Device:
    url = f"{BACKEND_URL}{serial_no}/"
    response = requests.get(url=url)
    return Device(**response.text)

def delete_device(serial_no: str) -> Device:
    url = f"{BACKEND_URL}{serial_no}/"
    response = requests.delete(url=url)
    print(f"Response: {response.status_code}")

