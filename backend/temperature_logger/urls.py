
from django.urls import path
from .views import temperature_log_view, temperature_log_list_view

urlpatterns = [
    path('', temperature_log_view),
    path('<str:pk>/', temperature_log_list_view),
]