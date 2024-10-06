from django.db import models
from django.core.exceptions import ValidationError

def validate_pk_length(value):
    if len(value) < 4:
        raise ValidationError('Primary key must be at least 4 characters long.')


# Create your models here.
class Device(models.Model):
    serial_no = models.CharField(
        max_length=30,
        primary_key=True,
        unique=True,
        validators=[validate_pk_length]
    )
    user_name = models.CharField(
        max_length=30,
        blank=True
    )
    user_location = models.CharField(
        max_length=30,
        blank=True
    )