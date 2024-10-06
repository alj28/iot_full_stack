from django.shortcuts import render

from django.http import HttpResponse, JsonResponse
from django.views.decorators.csrf import csrf_exempt
from rest_framework.parsers import JSONParser
from .models import TemperatureLog
from device.models import Device
from .serializers import TemperatureLogSerializer

def _error_response(msg: str, status: int = 400):
    return JsonResponse({'error'    :   f'{msg}'}, satus=status)

def _create_new_temperature_log(request):
    print("############################ " + str(request))
    data = JSONParser().parse(request)
    serializer = TemperatureLogSerializer(data=data)
    if serializer.is_valid():
        serializer.save()
        return JsonResponse(serializer.data, status=201)
    return _error_response(serializer.erros, status=400)

@csrf_exempt
def temperature_log_view(request):
    print(f"############# temperature_log_view #############")
    if request.method == 'POST':
        return _create_new_temperature_log(request)
    return _error_response(f'Method {request.method} not supported')

def _temperature_log_list_per_device(request, pk):
    try:
        device = Device.objects.get(pk=pk)
    except Device.DoesNotExist:
        return _error_response('Device does not exists', status=400)
    temperature_logs = TemperatureLog.objects.filter(device=device)
    serializer = TemperatureLogSerializer(temperature_logs, many=True)
    return JsonResponse(serializer.data, safe=False)

@csrf_exempt
def temperature_log_list_view(request, pk):
    print(f"############# temperature_log_list_view #############")
    if request.method == 'GET':
        return _temperature_log_list_per_device(request, pk)
    return _error_response(f'Method {request.method} not supported')


