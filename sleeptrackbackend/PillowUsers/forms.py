from django import forms
from .models import User


class UserForm(forms.ModelForm):
    username = forms.CharField()
    alarm_time = forms.TimeField()

    class Meta:
        model = User
        fields = ['username', 'alarm_time']
