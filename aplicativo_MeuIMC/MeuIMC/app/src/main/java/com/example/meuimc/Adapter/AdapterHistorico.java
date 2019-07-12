package com.example.meuimc.Adapter;

import android.graphics.Color;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Switch;
import android.widget.TextView;


import com.example.meuimc.Model.HistoricoModel;
import com.example.meuimc.R;

import java.util.ArrayList;
import java.util.List;

public class AdapterHistorico extends RecyclerView.Adapter<AdapterHistorico.MyViewHolder>{
    private List<HistoricoModel> listaHistorico;
    public AdapterHistorico(List<HistoricoModel> listaHistorico) {
        this.listaHistorico = new ArrayList<HistoricoModel>();
        this.listaHistorico = listaHistorico;
    }

    @NonNull
    @Override
    public MyViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        // Este método cria a View para serem exibidos os elementos

        // Converte o layout XML para uma View
        View listaItens = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.adapter_rv, viewGroup,false);

        return new MyViewHolder(listaItens);
    }

    @Override
    public void onBindViewHolder(@NonNull MyViewHolder myViewHolder, int i) {
        // Este método atualiza a visualização e mostra os elementos
        // i representa cada posição no RecyclerView
        // Como setamos a quantidade de elementos para lista.size()
        // i vai de 0 a lista.size

        myViewHolder.data.setText(listaHistorico.get(i).getData());
        myViewHolder.peso.setText((listaHistorico.get(i).getPeso()).toString());
        myViewHolder.altura.setText((listaHistorico.get(i).getAltura()).toString());
        String classificacao = listaHistorico.get(i).getClassificacao();
        myViewHolder.classificacao.setText(classificacao);

        if (classificacao.equals("Abaixo do peso")) myViewHolder.classificacao.setTextColor(Color.parseColor("#00ACC1"));
        else if(classificacao.equals("Peso ideal")) myViewHolder.classificacao.setTextColor(Color.parseColor("#7ecb20"));
        else if (classificacao.equals("Excesso de Peso")) myViewHolder.classificacao.setTextColor(Color.parseColor("#00ACC1"));
        else if (classificacao.equals("Obesidade de grau 1")) myViewHolder.classificacao.setTextColor(Color.parseColor("#FFB300"));
        else if (classificacao.equals("Obesidade de grau 2")) myViewHolder.classificacao.setTextColor(Color.parseColor("#FB8C00"));
        else myViewHolder.classificacao.setTextColor(Color.parseColor("#E53935"));

        String risco = listaHistorico.get(i).getRisco();
        myViewHolder.risco.setText(risco);
        if (risco.equals("Elevado")) myViewHolder.risco.setTextColor(Color.parseColor("#00ACC1"));
        else if(risco.equals("Inexistente")) myViewHolder.risco.setTextColor(Color.parseColor("#7ecb20"));
        else if (risco.equals("Elevado")) myViewHolder.risco.setTextColor(Color.parseColor("#00ACC1"));
        else if (risco.equals("Muito elevado")) myViewHolder.risco.setTextColor(Color.parseColor("#FFB300"));
        else if (risco.equals("Muitíssimo elevado")) myViewHolder.risco.setTextColor(Color.parseColor("#FB8C00"));
        else myViewHolder.risco.setTextColor(Color.parseColor("#E53935"));
    }

    @Override
    public int getItemCount() {
        // Retorna a quantidade de itens que serão exibidos
        return listaHistorico.size();
    }

    public class MyViewHolder extends RecyclerView.ViewHolder{
        //cria elementos gráficos que estarão no modelo
        TextView data;
        TextView peso;
        TextView altura;
        TextView classificacao;
        TextView risco;

        public MyViewHolder(@NonNull View itemView) {
            super(itemView);

            //linka os elementos do layout aos atributos da classe
            data = itemView.findViewById(R.id.tv_data);
            peso = itemView.findViewById(R.id.tv_peso);
            altura = itemView.findViewById(R.id.tv_altura);
            classificacao = itemView.findViewById(R.id.tv_classificacao);
            risco = itemView.findViewById(R.id.tv_risco);
        }
    }
}
