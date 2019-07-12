package com.example.meuimc.Activities;

import android.content.DialogInterface;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.support.design.widget.TextInputLayout;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.example.meuimc.DAO.DBSQLiteHelper;
import com.example.meuimc.Model.ContaModel;
import com.example.meuimc.R;

import org.w3c.dom.Text;

public class EsqueciSenhaActivity extends AppCompatActivity {
    Button btn;
    TextInputLayout til;
    EditText etEmail;
    TextView tv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_esqueci_senha);

        btn = findViewById(R.id.btn_recuperar);
        til = findViewById(R.id.til_email);
        etEmail = findViewById(R.id.et_email);
        tv = findViewById(R.id.textView7);
        tv.setText("");

        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String email = etEmail.getText().toString();
                DBSQLiteHelper db = new DBSQLiteHelper(getApplicationContext());

                ContaModel conta = db.getUser(email);
                if(email.isEmpty()){
                    til.setErrorEnabled(true);
                    til.setError("Insira um e-mail válido.");
                }else if(conta.getEmail() == null){
                    til.setErrorEnabled(true);
                    til.setError("E-mail digitado não foi encontrado.");
                } else{
                    til.setErrorEnabled(false);
                    sendEmail(conta);
                    tv.setText("Retornaremos em até 48h.");

                }

            }
        });

    }

    public void geraAlert() {
        AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        dialog.setTitle("Recuperação de senha");
        dialog.setMessage("A solicitação de senha foi encaminha à nossa central.\nRetornaremos ao e-mail digitado em até 48h.");
        dialog.setCancelable(false);
        dialog.setIcon(android.R.drawable.ic_dialog_alert);

        dialog.setPositiveButton("Sim", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
            }
        });

        dialog.create();
        dialog.show();
    }

    //Classe responsável por enviar um e-mail para applorraine01.gmail.com.
    //Esse e-mail foi criado no intuito de ser respondido posteriormente para o usuário com sua senha.
    protected void sendEmail(ContaModel user) {
        Log.i("Send email", "");

        String[] TO = {"applorraine01@gmail.com"};//user.getEmail()};
        String[] CC = {"lorraine.pepe@unesp.br"};

        Intent emailIntent = new Intent(Intent.ACTION_SEND);
        emailIntent.setData(Uri.parse("mailto:"));
        emailIntent.setType("text/plain");


        emailIntent.putExtra(Intent.EXTRA_EMAIL, TO);
        emailIntent.putExtra(Intent.EXTRA_CC, CC);
        emailIntent.putExtra(Intent.EXTRA_SUBJECT, "[MeuIMC] Recuperação de Senha");
        String mensagem = "O usuário "+user.getEmail()+", de nome "+user.getNome()+"\n solicitou a recuperação de senha do aplicativo MeuIMC.\n\n A senha é "+user.getPassword()+"\n\nE-mail enviado automaticamente.";
        emailIntent.putExtra(Intent.EXTRA_TEXT, mensagem);

        try {
            startActivity(Intent.createChooser(emailIntent, "Send mail..."));
            finish();
            Log.i("Sucesso", "Mensagem enviada");
            geraAlert();
        } catch (android.content.ActivityNotFoundException ex) {
            Toast.makeText(this,
                    "There is no email client installed.", Toast.LENGTH_SHORT).show();
        }

    }

}
