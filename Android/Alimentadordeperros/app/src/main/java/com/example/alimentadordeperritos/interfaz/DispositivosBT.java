package com.example.alimentadordeperritos.interfaz;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.example.alimentadordeperritos.R;
import com.example.alimentadordeperritos.bd.PerfilPerro;

import java.util.Set;

public class DispositivosBT extends AppCompatActivity {


    public static final int TODO_OK=1;
    public static final int TODO_MAL=0;

    private PerfilPerro perfilActual; //Aca se guarda el perfil


    //1)
    // Depuración de LOGCAT
    private static final String TAG = "DispositivosBT"; //<-<- PARTE A MODIFICAR >->->
    // Declaracion de ListView
    ListView IdLista;
    // String que se enviara a la actividad principal, mainactivity
    public static String EXTRA_DEVICE_ADDRESS = "device_address";

    // Declaracion de campos
    private BluetoothAdapter mBtAdapter;
    private ArrayAdapter mPairedDevicesArrayAdapter;

    private Bundle myBundle; //DECLARO ACA EL BUNDLE PARA DESPUES PODER ENVIAR EL PAQUETE A MainActivity!

    private boolean soyConfigManual;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dispositivos_bt);

        myBundle = this.getIntent().getExtras();



    }

    @Override
    public void onResume()
    {
        super.onResume();
        //---------------------------------
        VerificarEstadoBT();

        // Inicializa la array que contendra la lista de los dispositivos bluetooth vinculados
        mPairedDevicesArrayAdapter = new ArrayAdapter(this, R.layout.nombre_dispositivos);//<-<- PARTE A MODIFICAR >->->
        // Presenta los disposisitivos vinculados en el ListView
        IdLista = (ListView) findViewById(R.id.IdLista);
        IdLista.setAdapter(mPairedDevicesArrayAdapter);
        IdLista.setOnItemClickListener(mDeviceClickListener);
        // Obtiene el adaptador local Bluetooth adapter
        mBtAdapter = BluetoothAdapter.getDefaultAdapter();

        //------------------- EN CASO DE ERROR -------------------------------------
        //SI OBTIENES UN ERROR EN LA LINEA (BluetoothDevice device : pairedDevices)
        //CAMBIA LA SIGUIENTE LINEA POR
        //Set <BluetoothDevice> pairedDevices = mBtAdapter.getBondedDevices();
        //------------------------------------------------------------------------------

        // Obtiene un conjunto de dispositivos actualmente emparejados y agregua a 'pairedDevices'
        Set <BluetoothDevice> pairedDevices = mBtAdapter.getBondedDevices();

        // Adiciona un dispositivos previo emparejado al array
        if (pairedDevices.size() > 0)
        {
            for (BluetoothDevice device : pairedDevices) { //EN CASO DE ERROR LEER LA ANTERIOR EXPLICACION
                mPairedDevicesArrayAdapter.add(device.getName() + "\n" + device.getAddress());
            }
        }
    }

    // Configura un (on-click) para la lista
    private AdapterView.OnItemClickListener mDeviceClickListener = new AdapterView.OnItemClickListener() {
        public void onItemClick(AdapterView av, View v, int arg2, long arg3) {

            // Obtener la dirección MAC del dispositivo, que son los últimos 17 caracteres en la vista
            String info = ((TextView) v).getText().toString();
            String address = info.substring(info.length() - 17);

                // Realiza un intent para iniciar la siguiente actividad
            // mientras toma un EXTRA_DEVICE_ADDRESS que es la dirección MAC.

            try {
                Intent i;
                //soyConfigManual = myBundle.getBoolean("boolean");
                //if(soyConfigManual)
                //     i = new Intent(DispositivosBT.this, MainActivity.class);//iba temporal activity
                //else {
                    i = new Intent(DispositivosBT.this, MainActivity.class);

                    i.putExtras(myBundle); //GUARDO EN EL INTENT EL PERFIL DEL PERRO
                //}


                    i.putExtras(myBundle); //prueba borrar si crashea

                    i.putExtra(EXTRA_DEVICE_ADDRESS, address); //EN EL PAQUETE QUE TENGO GUARDADO EL PERFIL DEL PERRO, LE AGREGO LA DIRECCION MAC DEL DISPOSITIVO AL CUAL ME QUIERO CONECTAR.

                    startActivity(i);


         }catch(RuntimeException e)
         {
             Log.e("<<Error intent >>", e.getMessage());
         }
        }
    };


    private void VerificarEstadoBT() {
        // Comprueba que el dispositivo tiene Bluetooth y que está encendido.
        mBtAdapter= BluetoothAdapter.getDefaultAdapter();
        if(mBtAdapter==null) {
            Toast.makeText(getBaseContext(), "El dispositivo no soporta Bluetooth", Toast.LENGTH_SHORT).show();
        } else {
            if (mBtAdapter.isEnabled()) {
                Log.d(TAG, "...Bluetooth Activado...");
            } else {
                //Solicita al usuario que active Bluetooth
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);

            }
        }
    }

}


