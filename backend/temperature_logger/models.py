from django.db import models
from django.core.exceptions import ValidationError

from device.models import Device

class TemperatureLog(models.Model):
    device = models.ForeignKey(Device, on_delete=models.CASCADE)
    created_at = models.DateTimeField(auto_created=True)    # at creation store date and time
    temperature = models.FloatField()

    def __str__(self):
        return f"{self.device.user_name}: {self.temperature}"



    def save(self, *args, **kwargs):
        """
            Default save method overrided to prevent updating the
            record after creation.
        """
        if self.pk is not None:
            raise ValidationError("Record cannot be updated.")
        super(TemperatureLog, self).save(*args, **kwargs)