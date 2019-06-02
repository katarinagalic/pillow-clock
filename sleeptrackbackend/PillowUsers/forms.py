from django import forms
from .models import User_Alarm, Night


class UserAlarmForm(forms.ModelForm):
    user_id = forms.CharField()
    alarm_time = forms.TimeField(widget=forms.TextInput(attrs={'placeholder': 'HH:MM:SS'}))

    class Meta:
        model = User_Alarm
        fields = ['user_id', 'alarm_time']

class UserForm(forms.ModelForm):
    user_id = forms.CharField()

    class Meta:
        model = User_Alarm
        fields = ['user_id']


class NightForm(forms.ModelForm):
    sleeper = forms.ModelChoiceField(queryset=User_Alarm.objects.all())
    start_sleep = forms.DateTimeField(widget=forms.TextInput(attrs={'placeholder': 'YYYY-MM-DD HH:MM:SS'}))
    end_sleep = forms.DateTimeField(widget=forms.TextInput(attrs={'placeholder': 'YYYY-MM-DD HH:MM:SS'}))

    class Meta:
        model = Night
        fields = ['sleeper', 'start_sleep', 'end_sleep']
