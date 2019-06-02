from django.db import models
from datetime import datetime, timedelta

class User_Alarm(models.Model):

    user_id = models.CharField(max_length=100, unique=True)
    alarm_time = models.TimeField(null=True)

    def __str__(self):
        return self.user_id

    def getUser(self):
        return self.user_id


class Night (models.Model):
    sleeper = models.ForeignKey(User_Alarm, on_delete=models.CASCADE, null=False)
    day_id = models.IntegerField(null=True, blank=True)
    start_sleep = models.DateTimeField(auto_now=False, auto_now_add=False)
    end_sleep = models.DateTimeField(auto_now=False, auto_now_add=False)

    def conversion(self):
        return (self.end_sleep - self.start_sleep).total_seconds()/3600

    def save(self, *args, **kwargs):
        if not self.day_id:
            self.day_id = self.sleeper.night_set.count() + 1
        super(Night, self).save(*args, **kwargs)

    def __str__(self):
        return str(self.day_id) + " " + str(self.conversion())

