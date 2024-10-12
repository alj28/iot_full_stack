
from django.urls import path
from .views import device_view, device_details_view

urlpatterns = [
    path('', device_view),
    path('<str:pk>/', device_details_view)
]