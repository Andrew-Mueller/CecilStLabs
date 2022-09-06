
#ifndef SENSORREADINGTHREAD_H_
#define SENSORREADINGTHREAD_H_

namespace Terso
{

   class SensorReadingThread : public PeriodicThread, public ILoggable
   {
      public:

         /**
          * Create the Sensor Reading Thread with the specified device access data
          * and number of seconds to define the heartbeat's period.
          *
          * @param deviceAccessKey The guid that provides access to post
          *        messages for the device.
          *
          * @param deviceSerialNumber The human readable serial number for the
          *        device for example: NMB000100001
          *
          * @param commQueue The communication queue for sending messages to the
          *        Jetstream server.
          *
          * @param clockDriver The clock for grabbing the time of the sensor reading event.
          *
          * @param sensorReadingPeriod_minutes The number of minutes to wait
          *        between posting the sensor reading threads.
          */
         SensorReadingThread(std::string deviceAccessKey,
                             std::string deviceSerialNumber,
                             CommQueue* commQueue,
                             IClockDriver* clockDriver,
                             uint16_t sensorReadingPeriod_minutes);

         /**
          * release the resources used by the Sensor reading thread.
          */
         virtual ~SensorReadingThread();

         /**
          * Add a sensor to read periodically and publish to jetstream in the
          * SensorReadingEvent.
          *
          * @param sensor Sensor to read periodically and publish to Jetstream.
          */
         void addSensor(ISensor* sensor);


         inline void setTemperatureAlarm() { m_inTemperatureAlarm = true; };
         inline void clearTemperatureAlarm() { m_inTemperatureAlarm = false; };

         /**
          * Sets the number of minutes that the periodic thread waits between
          * calling its periodic async worker method.
          *
          * NOTE: The thread will restart from minute zero with the new number
          *       of minutes.
          *
          * @param period The new number of seconds the thread will wait between
          *               doing its work via its PeriodicAsyncWofrker function.
          *
          * @param periodInterval The interval to interpret the period as
          *                       (minutes, seconds, etc)
          */
         void SetPeriod(const uint16_t period, const ePeriodInterval periodInterval);

         /**
          * Overridden method of the parent, PeriodicThread, that defines the
          * action to take for the thread to do its work.
          */
         void PeriodicAsyncWorker();

      protected:

         /**
          * flag if the current sensor reading system is in the temperature alarm.
          */
         bool m_inTemperatureAlarm;

         /**
          * The Jetstream access key for the device
          */
         string m_deviceAccessKey;

         /**
          * The Serial number of the device to send the heartbeat.
          */
         string m_deviceSerialNumber;

         /**
          * Communication queue used for sending message to the Jetstream Server
          * and persisting data if communication network is down.
          */
         CommQueue* m_commQueue;

         /**
          * Clock driver used to get the current time required for the sensor
          * reading event.
          */
         IClockDriver* m_clockDriver;

         /**
          * Specific Jetstream Device URL to send the Sensor Reading Event.
          */
         std::string m_sensorReadingURL;

         /**
          * The body of the HTTP request used to post the sensor reading event.
          */
         std::string m_sensorReadingBody;

         /**
          * jetstream event used for reading sensors and generating the event XML.
          */
         SensorReadingEvent m_sensorReadingEvent;
   };

}
#endif
