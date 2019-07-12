package com.example.meuimc.DAO;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;
import android.widget.Toast;

import com.example.meuimc.Model.ContaModel;
import com.example.meuimc.Model.HistoricoModel;

import java.util.ArrayList;
import java.util.List;

public class DBSQLiteHelper extends SQLiteOpenHelper{

    private static final int DATABASE_VERSION = 1;
    private static final String DATABASE_NAME = "imcDB";

    public DBSQLiteHelper(Context context){
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    public void onCreate(SQLiteDatabase db){
        db.execSQL("CREATE TABLE IF NOT EXISTS tb_contas (id INTEGER PRIMARY KEY, email VARCHAR NOT NULL, nome VARCHAR NOT NULL, senha VARCHAR NOT NULL)");
        db.execSQL("CREATE TABLE IF NOT EXISTS tb_historico (id INTEGER PRIMARY KEY, email VARCHAR NOT NULL, data VARCHAR NOT NULL, peso DOUBLE NOT NULL, altura DOUBLE NOT NULL, FOREIGN KEY(id) REFERENCES tb_contas(id))");
    }

    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion){
        db.execSQL("DROP TABLE IF EXISTS tb_contas");
        db.execSQL("DROP TABLE IF EXISTS tb_historico");
        this.onCreate(db);
    }

    public void addUser(ContaModel conta){
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues v = new ContentValues();
        v.put("email", conta.getEmail());
        v.put("nome", conta.getNome());
        v.put("senha", conta.getPassword());

        db.insert("tb_contas", null, v);
        db.close();
    }

    public ContaModel getUser(String email){
        //Busca e retorna a conta de um user.
        String selectQuery = "SELECT * FROM tb_contas WHERE email = ?";
        ContaModel conta = new ContaModel();
        SQLiteDatabase db = this.getWritableDatabase();

        Cursor cursor = db.rawQuery(selectQuery, new String[] { email });
        if(cursor.moveToFirst()){
            conta.setEmail(cursor.getString(cursor.getColumnIndex("email")));
            conta.setNome(cursor.getString(cursor.getColumnIndex("nome")));
            conta.setPassword(cursor.getString(cursor.getColumnIndex("senha")));
        }
        cursor.close();
        return conta;

    }

    public void editaUser(ContaModel conta){
        //Edita a conta do usuário
        SQLiteDatabase db = this.getWritableDatabase();
        String email = conta.getEmail();
        ContentValues v = new ContentValues();
        v.put("nome", conta.getNome());
        v.put("senha",conta.getPassword());

        db.update("tb_contas", v, "email = ?", new String[]{email});
    }

    public void deletaUser(String email){
        SQLiteDatabase db = this.getWritableDatabase();
        //Deleta todo o histórico do usuário e em seguida a conta
        int j = db.delete("tb_historico", "email = ?", new String[]{email});
        int i = db.delete("tb_contas","email = ?", new String[]{email});
    }

    public void addItemHistorico(HistoricoModel h){
        //Adiciona o item calculado no CalculoFragment, ao histórico do usuário.
        //O id é inserido automaticamente.
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues v = new ContentValues();
        v.put("email",h.getEmail());
        v.put("data",h.getData());
        v.put("peso",h.getPeso());
        v.put("altura",h.getAltura());

        db.insert("tb_historico", null, v);
        db.close();
    }

    public ArrayList<HistoricoModel> getHistorico(String email){
        //Busca e retorna o histórico do usuário
        ArrayList<HistoricoModel> listHistorico = new ArrayList<>();
        String selectQuery = "SELECT data, peso, altura FROM tb_historico WHERE email = ? ORDER BY id DESC";
        SQLiteDatabase db = this.getWritableDatabase();

        Cursor cursor = db.rawQuery(selectQuery, new String[] { email });

        if (cursor.getCount() > 0) {
            while(cursor.moveToNext()){
                String data = cursor.getString(cursor.getColumnIndex("data"));
                Double peso = cursor.getDouble(cursor.getColumnIndex("peso"));
                Double altura = cursor.getDouble(cursor.getColumnIndex("altura"));
                listHistorico.add(new HistoricoModel(email, data, peso, altura));
            }
        }
        return listHistorico;

    }

}
