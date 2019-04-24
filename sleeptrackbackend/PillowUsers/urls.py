from django.urls import path
from . import views

urlpatterns = [
    path('new_user/', views.create_user, name='new_user'),
    path('edit/<str:current>', views.edit_user, name='edit_user'),
    path('', views.user_list, name='user_list')
]