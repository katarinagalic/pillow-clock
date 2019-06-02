from django.urls import path
from . import views

urlpatterns = [
    path('new_user/', views.create_user, name='new_user'),
    path('edit/<str:current>', views.edit_user, name='edit_user'),
    path('users/', views.user_list, name='user_list'),
    path('about/', views.about_page, name='about'),
    path('sleeptrack/<str:current>', views.sleeptrack, name='sleeptrack'),
    path('nights/', views.nights, name='nights'),
    path('system/', views.hardware_page, name='system'),
    path('', views.about_page, name='user_list')
]