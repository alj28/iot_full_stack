from django.shortcuts import render

from django.http import HttpResponse, JsonResponse
from django.views.decorators.csrf import csrf_exempt
from rest_framework.parsers import JSONParser
from .models import Device
from .serializers import DeviceSerializer


def _error_response(msg: str, status: int = 400):
    return JsonResponse({'error'    :   f'{msg}'}, satus=status)

def _create_new_device(request):
    data = JSONParser().parse(request)
    serializer = DeviceSerializer(data=data)
    if serializer.is_valid():
        serializer.save()
        return JsonResponse(serializer.data, status=201)
    return _error_response(serializer.errors, status=400)

def _list_all_devics(request):
    all_devices = Device.objects.all()
    serializer = DeviceSerializer(all_devices, many=True)
    return JsonResponse(serializer.data, safe=False)

@csrf_exempt    # TODO: remove this as this is not safe
def device_view(request):
    HANDLERS = {
        'GET'   :   _list_all_devics,
        'POST'  :   _create_new_device
    }
    if request.method in HANDLERS:
        return HANDLERS[request.method](request)
    return _error_response(f'Method {request.method} not supported')

def _get_device_details(device, request):
    serializer = DeviceSerializer(device)
    return JsonResponse(serializer.data)

def _update_device_details(device, request):
    data = JSONParser().parse(request)
    serializer = DeviceSerializer(device, data=data)
    if serializer.is_valid():
        serializer.save()
        return JsonResponse(serializer.data)
    return _error_response(serializer.errors, status=400) 

def _delete_device(device, request):
    device.delete()
    return HttpResponse(status=204)

@csrf_exempt    # TODO: remove this as this is not safe
def device_details_view(request, pk):
    print(f"##################### {pk}")
    HANDLERS = {
        'GET'       :   _get_device_details,
        'PUT'       :   _update_device_details,
        'DELETE'    :   _delete_device
    }
    if request.method in HANDLERS:
        try:
            device = Device.objects.get(pk=pk)
        except Device.DoesNotExist:
            return _error_response('Device does not exists', status=204)
        return HANDLERS[request.method](device, request)
    return _error_response(f'Method {request.method} not supported')
