package com.example.meuimc.Activities;

import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.Color;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AlertDialog;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.example.meuimc.Adapter.AdapterHistorico;
import com.example.meuimc.DAO.DBSQLiteHelper;
import com.example.meuimc.Model.ContaModel;
import com.example.meuimc.Model.HistoricoModel;
import com.example.meuimc.R;

import java.util.ArrayList;
import java.util.List;

@SuppressLint("ValidFragment")
public class HistoricoFragment extends Fragment {

    private RecyclerView recyclerDados;
    private List<HistoricoModel> itemHistorico = new ArrayList<HistoricoModel>();
    private String email, senha;
    private DBSQLiteHelper db;

    @SuppressLint("ValidFragment")
    public HistoricoFragment(String email, String senha){
        this.email = email;
        this.senha = senha;
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
       View view =  inflater.inflate(R.layout.fragment_historico, container, false);
       recyclerDados = view.findViewById(R.id.rv);

        //pega dados da Activity responsável por esse fragmento (IMC Activity)
        IMCActivity activity = (IMCActivity) getActivity();
        email = activity.retornaEmail();
        senha = activity.retornaSenha();

        // Listagem do historico
        criarHistorico(email);

        // Configurar adapter
        AdapterHistorico adapter = new AdapterHistorico(itemHistorico);

        // Configurar RecyclerView
        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager(getActivity().getApplicationContext());
        recyclerDados.setLayoutManager(layoutManager);
        // fixa o tamanho para otimizar
        recyclerDados.setHasFixedSize(true);
        // adiciona linha separadora dos elementos
        recyclerDados.addItemDecoration(new DividerItemDecoration(getContext(), LinearLayout.VERTICAL));

        recyclerDados.setAdapter(adapter);
        if (adapter.getItemCount() == 0) {
            geraAlert();
        }

        return view;
    }

    void criarHistorico(String email){
    //pesquisa banco, ordenar do mais novo ao mais antigo (ordem de id).
        db= new DBSQLiteHelper(getContext());
        ArrayList<HistoricoModel> listHistorico = db.getHistorico(email);
        if(listHistorico != null) {
            /*for (HistoricoModel hist : listHistorico) {
                itemHistorico.add(new HistoricoModel(hist.getEmail(), hist.getData(), hist.getPeso(), hist.getAltura()));
            }*/
            itemHistorico = listHistorico;
        }
    }

    public void geraAlert() {
        AlertDialog.Builder dialog = new AlertDialog.Builder(getContext());
        dialog.setTitle("Histórico vazio.");
        dialog.setMessage("Os IMCs calculados podem ser encontrados aqui posteriormente.");
        dialog.setCancelable(false);
        dialog.setIcon(android.R.drawable.ic_dialog_info);
        dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                // Histórico vazio, redireciona para o fragmento CalculoFragment
                Fragment newFragment = new CalculoFragment();
                FragmentTransaction transaction = getActivity().getSupportFragmentManager().beginTransaction();
                transaction.replace(R.id.fragment_container, newFragment);
                transaction.addToBackStack(null);
                transaction.commit();
            }
        });

        dialog.create();
        dialog.show();
    }

}
