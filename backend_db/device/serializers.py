
from rest_framework import serializers
from .models import Device

class DeviceSerializer(serializers.Serializer):
    serial_no = serializers.CharField(required=True, max_length=30)
    user_name = serializers.CharField(required=False, allow_blank=True, max_length=30)
    user_location = serializers.CharField(required=False, allow_blank=True, max_length=30)

    def create(self, validated_data):
        return Device.objects.create(**validated_data)
    
    def update(self, instance, validated_data):
        instance.serial_no = validated_data.get('serial_no')
        instance.user_name = validated_data.get('user_name')
        instance.user_location = validated_data.get('user_location')
        instance.save()
        return instance