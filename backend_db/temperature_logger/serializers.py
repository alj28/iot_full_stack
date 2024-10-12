
from rest_framework import serializers
from .models import TemperatureLog, Device

class TemperatureLogSerializer(serializers.Serializer):
    device = serializers.CharField()
    created_at = serializers.DateTimeField(read_only=True)
    temperature = serializers.FloatField()

    def create(self, validated_data):
        device_serial_no = validated_data.get('device')
        temperature = validated_data.get('temperature')
        device = Device.objects.get(serial_no=device_serial_no)
        temperature_log = TemperatureLog.objects.create(
            device = device,
            temperature = temperature
        )
        return temperature_log

    def validate_device(self, value):
        if not Device.objects.filter(serial_no=value).exists():
            raise serializers.ValidationError(f"Device with serial number {value} does not exists")
        return value
    
    def to_representation(self, instance):
        representation = super().to_representation(instance)
        #representation['device'] = instance.device.serial_no    # user friendly representation of foreing-key
        representation.pop('device')                            # removing of device serial number as it is already in the request
        return representation

